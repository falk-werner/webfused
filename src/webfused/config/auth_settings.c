#include "webfused/auth/settings.h"
#include "webfused/config/auth_settings.h"

#include <libconfig.h>
#include <stdlib.h>
#include <string.h>

struct wfd_auth_settings
{
    char * provider_name;
    struct config_setting_t * settings;
};

struct wfd_auth_settings *
wfd_auth_settings_create(
    char const * provider_name,
    struct config_setting_t * settings)
{
    struct wfd_auth_settings * auth_settings = malloc(sizeof(struct wfd_auth_settings));
    auth_settings->provider_name = strdup(provider_name);
    auth_settings->settings = settings;

    return auth_settings;
}

void
wfd_auth_settings_dispose(
    struct wfd_auth_settings * settings)
{
    free(settings->provider_name);
    free(settings);
}

char const *
wfd_auth_settings_get_provider(
    struct wfd_auth_settings * settings)
{
    return settings->provider_name;
}

char const *
wfd_auth_settings_get(
    struct wfd_auth_settings * settings,
    char const * key)
{
    char const * result;
    int rc = config_setting_lookup_string(settings->settings, key, &result);

    return (CONFIG_TRUE == rc) ? result : NULL;
}

