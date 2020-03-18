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
wfd_settings_get_string(
    struct wfd_settings * settings,
    char const * key)
{
    if (NULL == settings) {return NULL; }

    char const * result;
    int rc = config_setting_lookup_string(settings->setting, key, &result);

    return (CONFIG_TRUE == rc) ? result : NULL;
}

char const *
wfd_settings_get_string_or_default(
    struct wfd_settings * settings,
    char const * key,
    char const * default_value)
{
    if (NULL == settings) {return default_value; }

    char const * result;
    int rc = config_setting_lookup_string(settings->setting, key, &result);

    return (CONFIG_TRUE == rc) ? result : default_value;
}

bool
wfd_settings_get_bool(
    struct wfd_settings * settings,
    char const * key)
{
    if (NULL == settings) {return false; }

    int result;
    int rc = config_setting_lookup_bool(settings->setting, key, &result);

    return ((CONFIG_TRUE == rc) && (CONFIG_TRUE == result));
}
