#include "webfused/config/settings.h"
#include "webfused/config/settings_intern.h"

#include <libconfig.h>
#include <stddef.h>

void
wfd_settings_init(
    struct wfd_settings * settings,
    struct config_setting_t * setting)
{
    settings->setting = setting;
}

void
wfd_settings_cleanup(
    struct wfd_settings * settings)
{
    settings->setting = NULL;
}

char const *
wfd_settings_get(
    struct wfd_settings * settings,
    char const * key)
{
    char const * result;
    int rc = config_setting_lookup_string(settings->setting, key, &result);

    return (CONFIG_TRUE == rc) ? result : NULL;
}

