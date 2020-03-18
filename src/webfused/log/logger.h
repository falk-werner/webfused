#ifndef WFD_LOGGER_H
#define WFD_LOGGER_H

#ifndef __cplusplus
#include <stdarg.h>
#else
#include <cstdarg>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef void 
wfd_logger_log_fn(
    void * user_data,
    int level,
    char const * format,
    va_list args);

typedef void
wfd_logger_onclose_fn(
    void * user_data);

extern void
wfd_logger_init(
    int level,
    wfd_logger_log_fn * log,
    wfd_logger_onclose_fn * onclose,
    void * user_data);

extern void
wfd_logger_close(void);

#ifdef __cplusplus
}
#endif

#endif
