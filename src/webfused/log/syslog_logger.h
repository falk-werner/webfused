#ifndef WFD_LOG_SYSLOG_LOGGER_H
#define WFD_LOG_SYSLOG_LOGGER_H

#ifdef __cplusplus
extern "C"
 {
#endif

extern void
wfd_syslog_logger_init(
    int level,
    char const * ident,
    int options,
    int facility);

#ifdef __cplusplus
 }
#endif

#endif
