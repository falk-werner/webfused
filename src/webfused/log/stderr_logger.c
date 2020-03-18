#include "webfused/log/stderr_logger.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"

#include <stdio.h>
#include <stdlib.h>

static void 
wfd_stderr_logger_log(
    void * user_data,
    int level,
    char const * format,
    va_list args)
{
    fprintf(stderr, "%s: ", wfd_log_level_tostring(level));
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}

bool
wfd_stderr_logger_init(
    int level,
    struct wfd_settings * settings)
{
    (void) settings;
    
    wfd_logger_init(level, &wfd_stderr_logger_log, NULL, NULL);
    return true;
}
