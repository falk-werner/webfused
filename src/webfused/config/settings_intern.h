#ifndef WFD_CONFIG_SETTINGS_INTERN_H
#define WFD_CONFIG_SETTINGS_INTERN_H

#ifdef __cplusplus
extern "C"
{
#endif

struct config_setting_t;

struct wfd_settings
{
    struct config_setting_t * setting; 
};


extern void
wfd_settings_init(
    struct wfd_settings * settings,
    struct config_setting_t * setting);

extern void
wfd_settings_cleanup(
    struct wfd_settings * settings);

#ifdef __cplusplus
}
#endif

#endif
