#include "webfused/auth/factory.h"
#include "webfused/auth/settings.h"
#include "webfused/auth/file_authenticator.h"
#include "webfused/log/log.h"

#include <string.h>

bool
wfd_authenticator_create(
    struct wfd_auth_settings * settings,
    struct wfd_authenticator * authenticator)
{
    bool result = false;
    char const * provider_name = wfd_auth_settings_get_provider(settings);
    if (0 == strcmp("file", provider_name))
    {
        result = wfd_file_authenticator_create(settings, authenticator);
    }
    else
    {
        WFD_ERROR("failed to create authenticator: unknown type \"%s\"", provider_name);
    }

    return result;
}
