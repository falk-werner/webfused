#ifndef WFD_CONFIG_AUTH_SETTINGS_H
#define WFD_CONFIG_AUTH_SETTINGS_H

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_auth_settings;
struct config_setting_t;

extern struct wfd_auth_settings *
wfd_auth_settings_create(
    char const * provider_name,
    struct config_setting_t * settings);

extern void
wfd_auth_settings_dispose(
    struct wfd_auth_settings * settings);

#ifdef __cplusplus
}
#endif

#endif
