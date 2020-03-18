#ifndef WFD_LOG_MANAGER_H
#define WFD_LOG_MANAGER_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_settings;

extern bool
wfd_log_manager_set_logger(
    char const * provider,
    int level,
    struct wfd_settings * settings);

#ifdef __cplusplus
}
#endif

#endif
