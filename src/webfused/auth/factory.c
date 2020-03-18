#include "webfused/auth/factory.h"
#include "webfused/auth/file_authenticator.h"
#include "webfused/config/settings.h"
#include "webfused/log/log.h"

#include <string.h>

bool
wfd_authenticator_create(
    char const * provider,
    struct wfd_settings * settings,
    struct wfd_authenticator * authenticator)
{
    bool result = false;
    if (0 == strcmp("file", provider))
    {
        result = wfd_file_authenticator_create(settings, authenticator);
    }
    else
    {
        WFD_ERROR("failed to create authenticator: unknown type \"%s\"", provider);
    }

    return result;
}
