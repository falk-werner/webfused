#ifndef WFD_CONFIG_SETTINGS_H
#define WFD_CONFIG_SETTINGS_H

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_settings;


extern char const *
wfd_settings_get(
    struct wfd_settings * settings,
    char const * key);

#ifdef __cplusplus
}
#endif


#endif
