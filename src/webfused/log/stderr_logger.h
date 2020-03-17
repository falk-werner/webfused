#ifndef WFD_LOG_STDERR_LOGGER_H
#define WFD_LOG_STDERR_LOGGER_H

#ifndef __cplusplus
#include <stdarg.h>
#else
#include <cstdarg>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern void 
wfd_stderr_logger_log(
    void * user_data,
    int level,
    char const * format,
    va_list args);


extern void
wfd_stderr_logger_init(
    int level);

#ifdef __cplusplus
}
#endif

#endif
