#ifndef WFD_CONFIG_FACTORY_H
#define WFD_CONFIG_FACTORY_H

#ifdef __cplusplus
extern "C"
{
#endif

extern struct wfd_config *
wfd_config_load_file(
    char const * filename);

extern struct wfd_config *
wfd_config_load_string(
    char const * contents);

#ifdef __cplusplus
}
#endif

#endif
