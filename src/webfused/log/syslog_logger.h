#ifndef WFD_LOG_SYSLOG_LOGGER_H
#define WFD_LOG_SYSLOG_LOGGER_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
 {
#endif

struct wfd_settings;

extern bool
wfd_syslog_logger_init(
    int level,
    struct wfd_settings * settings);

#ifdef __cplusplus
 }
#endif

#endif
