#include "webfused/auth/file_authenticator.h"
#include "webfused/auth/authenticator.h"
#include "webfused/config/settings.h"
#include "userdb/userdb.h"
#include "webfused/log/log.h"

#include <webfuse/adapter/credentials.h>

#include <stdlib.h>
#include <string.h>

struct wfd_file_authenticator
{
    char * filename;
};

static void
wfd_file_authenticator_dispose(
    void * data)
{
    struct wfd_file_authenticator * authenticator = data;

    free(authenticator->filename);
    free(authenticator);
}

static bool 
wfd_file_authenticator_authenticate(
    struct wf_credentials * credentials,
    void * user_data)
{
    bool result = false;
    struct wfd_file_authenticator * authenticator = user_data;

	char const * username = wf_credentials_get(credentials, "username");
	char const * password = wf_credentials_get(credentials, "password");
	if ((NULL != username) && (NULL != password))
	{
		struct userdb * db = userdb_create("");
		result = userdb_load(db, authenticator->filename); 
		if (result)
		{
			result = userdb_check(db, username, password);
		}

		userdb_dispose(db);
	}

    WFD_INFO("authenticate user \'%s\': %s", 
        (NULL != username) ? username : "<unknown>",
        result ? "success" : "failure");

    return result;
}

static char const *
wfd_file_authenticator_get_type(
    void * data)
{
    (void) data;
    return "username";  
}

static struct wfd_authenticator_vtable
wfd_file_authenticator_vtable =
{
    .dispose = &wfd_file_authenticator_dispose,
    .authenticate = &wfd_file_authenticator_authenticate,
    .get_type = &wfd_file_authenticator_get_type
};

bool
wfd_file_authenticator_create(
    struct wfd_settings * settings,
    struct wfd_authenticator * authenticator)
{
    bool result = false;

    char const * filename = wfd_settings_get(settings, "file");
    if (NULL != filename)
    {
        struct wfd_file_authenticator * data = malloc(sizeof(struct wfd_file_authenticator));
        data->filename = strdup(filename);

        authenticator->vtable = &wfd_file_authenticator_vtable;
        authenticator->data = data;
        result = true;
    }

    return result;
}
