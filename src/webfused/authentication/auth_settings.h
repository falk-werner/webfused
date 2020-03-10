#ifndef WFD_AUTH_SETTINGS_H
#define WFD_AUTH_SETTINGS_H

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_auth_settings;


extern char const *
wfd_auth_settings_get_provider(
    struct wfd_auth_settings * settings);

extern char const *
wfd_auth_settings_get(
    struct wfd_auth_settings * settings,
    char const * key);





#ifdef __cplusplus
}
#endif


#endif
