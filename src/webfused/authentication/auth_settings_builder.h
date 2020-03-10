#ifndef WFD_AUTH_SETTINGS_BUILDER_H
#define WFD_AUTH_SETTINGS_BUILDER_H

#ifdef __cplusplus
#endif

extern struct wfd_auth_settings *
wfd_auth_settings_create(void);

extern void
wfd_auth_settings_dispose(
    struct wfd_auth_settings * settings);

extern void
wfd_auth_settings_set_provider(
    struct wfd_auth_settings * settings,
    char const * provider);

extern void
wfd_auth_settings_add(
    struct wfd_auth_settings * settings,
    char const * key,
    char const * value);

#ifdef __cplusplus
#endif

#endif
