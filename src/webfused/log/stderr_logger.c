#include "webfused/log/stderr_logger.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"

#include <stdio.h>
#include <stdlib.h>

static const char *
wfd_stderr_logger_level_str(int level)
{
    switch (level)
    {
        case WFD_LOGLEVEL_FATAL: return "fatal";
        case WFD_LOGLEVEL_ERROR: return "error";
        case WFD_LOGLEVEL_WARN: return "warn";
        case WFD_LOGLEVEL_INFO: return "info";
        case WFD_LOGLEVEL_DEBUG: return "debug";
        default: return "notice";
    }
}

void 
wfd_stderr_logger_log(
    void * user_data,
    int level,
    char const * format,
    va_list args)
{
    fprintf(stderr, "%s: ", wfd_stderr_logger_level_str(level));
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}


void
wfd_stderr_logger_init(
    int level)
{
    wfd_logger_init(level, &wfd_stderr_logger_log, NULL, NULL);
}
