#ifndef WFD_CONFIG_SETTINGS_H
#define WFD_CONFIG_SETTINGS_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_settings;


extern char const *
wfd_settings_get_string(
    struct wfd_settings * settings,
    char const * key);

extern char const *
wfd_settings_get_string_or_default(
    struct wfd_settings * settings,
    char const * key,
    char const * default_value);

extern bool
wfd_settings_get_bool(
    struct wfd_settings * settings,
    char const * key);

#ifdef __cplusplus
}
#endif


#endif
