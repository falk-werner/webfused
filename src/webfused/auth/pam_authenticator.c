#include "webfused/auth/pam_authenticator.h"
#include "webfused/auth/authenticator.h"
#include "webfused/config/settings.h"
#include "webfused/log/log.h"

#include "webfuse/adapter/credentials.h"

#include <security/pam_appl.h>
#include <stdlib.h>
#include <string.h>

struct wfd_pam_authenticator
{
    char * service_name;
};

struct wfd_pam_credentials
{
    char const * username;
    char const * password;
};

static int 
wfd_pam_conversation(
    int count,
    struct pam_message const ** messages,
    struct pam_response * * ret_responses,
    void * user_data)
{
    int result = PAM_SUCCESS;
    struct pam_response * responses = malloc(count * sizeof(struct pam_response));
    struct wfd_pam_credentials * creds = user_data;

    for(int i = 0; (PAM_SUCCESS == result) && (i < count); i++)
    {
        struct pam_response * response = &responses[i];
        struct pam_message const * message = messages[i];

        response->resp_retcode = 0;
        response->resp = NULL;

        switch (message->msg_style)
        {
            case PAM_PROMPT_ECHO_ON:
                response->resp = strdup(creds->username);
                break;
            case PAM_PROMPT_ECHO_OFF:
                response->resp = strdup(creds->password);
                break;
            default:
                free(responses);
                result = PAM_CONV_ERR;
        }
    }

    if (PAM_SUCCESS == result)
    {
        *ret_responses = responses;
    }

    return result;
}

static void
wfd_pam_authenticator_dispose(
    void * data)
{
    struct wfd_pam_authenticator * authenticator = data;

    free(authenticator->service_name);
    free(authenticator);
}

static bool 
wfd_pam_authenticator_authenticate(
    struct wf_credentials * credentials,
    void * user_data)
{
    bool result = false;
    struct wfd_pam_authenticator * authenticator = user_data;

	char const * username = wf_credentials_get(credentials, "username");
	char const * password = wf_credentials_get(credentials, "password");
	if ((NULL != username) && (NULL != password))
    {
        struct wfd_pam_credentials creds =
        {
            .username = username,
            .password = password
        };

        struct pam_conv const conversation =
        {
            .conv = &wfd_pam_conversation,
            .appdata_ptr = &creds
        };

        pam_handle_t * handle = NULL;
        bool cleanup_handle = false;
        result = true;
        {
            int rc = pam_start(authenticator->service_name, NULL, 
                &conversation, &handle);
            result = (PAM_SUCCESS == rc);
            cleanup_handle = result; 
            if (!result)
            {
                WFD_ERROR("failed to start pam conversation: %s", pam_strerror(handle, rc));
            }
        }

        if (result)
        {
            int rc = pam_authenticate(handle, PAM_DISALLOW_NULL_AUTHTOK);
            result = (PAM_SUCCESS == rc);
            if (!result)
            {
                WFD_INFO("failed to authenticate user \'%s\' (pam_authenticate): %s", 
                    username, pam_strerror(handle, rc));
            }
        }

        if (result)
        {
            int rc = pam_acct_mgmt(handle, PAM_DISALLOW_NULL_AUTHTOK);
            result = (PAM_SUCCESS == rc);
            if (!result)
            {
                WFD_INFO("failed to authenticate user \'%s\' (pam_acct_mgmt): %s", 
                    username, pam_strerror(handle, rc));
            }
        }

        if (cleanup_handle)
        {
            pam_end(handle, 0);
        }
    }

    WFD_INFO("authenticate user \'%s\': %s", 
        (NULL != username) ? username : "<unknown>",
        result ? "success" : "failure");

    return result;
}

static char const *
wfd_pam_authenticator_get_type(
    void * data)
{
    (void) data;
    return "username";  
}

static struct wfd_authenticator_vtable const 
wfd_pam_authenticator_vtable =
{
    .dispose = &wfd_pam_authenticator_dispose,
    .authenticate = &wfd_pam_authenticator_authenticate,
    .get_type = &wfd_pam_authenticator_get_type
};

bool
wfd_pam_authenticator_create(
    struct wfd_settings * settings,
    struct wfd_authenticator * authenticator)
{
    struct wfd_pam_authenticator * data = malloc(sizeof(struct wfd_pam_authenticator));
    data->service_name  = strdup(wfd_settings_get_string_or_default(settings, "service_name", "webfused"));

    authenticator->vtable = &wfd_pam_authenticator_vtable;
    authenticator->data = data;
    return true;
}
