#ifndef WFD_LOG_H
#define WFD_LOG_H

#ifndef __cplusplus
#include <stdbool.h>
#include <stdarg.h>
#else
#include <cstdarg>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef WFD_LOGLEVEL
#define WFD_LOGLEVEL WFD_LOGLEVEL_ALL
#endif

#define WFD_LOGLEVEL_NONE -1
#define WFD_LOGLEVEL_FATAL 1
#define WFD_LOGLEVEL_ERROR 3
#define WFD_LOGLEVEL_WARN  4
#define WFD_LOGLEVEL_INFO  5
#define WFD_LOGLEVEL_DEBUG 7
#define WFD_LOGLEVEL_ALL   8

#define WFD_FATAL(...) \
    WFD_LOG(WFD_LOGLEVEL_FATAL, __VA_ARGS__)

#define WFD_ERROR(...) \
    WFD_LOG(WFD_LOGLEVEL_ERROR, __VA_ARGS__)

#define WFD_WARN(...) \
    WFD_LOG(WFD_LOGLEVEL_WARN, __VA_ARGS__)

#define WFD_INFO(...) \
    WFD_LOG(WFD_LOGLEVEL_INFO, __VA_ARGS__)

#define WFD_DEBUG(...) \
    WFD_LOG(WFD_LOGLEVEL_DEBUG, __VA_ARGS__)

#define WFD_LOG(level, ...) \
    do { \
        if (WFD_LOGLEVEL >= (level)) { \
            wfd_log((level), __VA_ARGS__); \
        } \
    } while (0)

extern void
wfd_log(
    int level,
    char const * format,
    ...);

extern char const *
wfd_log_level_tostring(int level);

extern bool
wfd_log_level_parse(
    char const * level,
    int * result);

#ifdef __cplusplus
}
#endif

#endif
