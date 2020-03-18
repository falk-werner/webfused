#include "webfused/log/syslog_logger.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"

#include <syslog.h>
#include <stddef.h>

static int
wfd_syslog_logger_to_priority(
    int level)
{
    switch (level)
    {
        case WFD_LOGLEVEL_FATAL: return LOG_CRIT;
        case WFD_LOGLEVEL_ERROR: return LOG_ERR;
        case WFD_LOGLEVEL_WARN: return LOG_WARNING;
        case WFD_LOGLEVEL_INFO: return LOG_INFO;
        case WFD_LOGLEVEL_DEBUG: return LOG_DEBUG;
        default: return LOG_NOTICE;
    }
}

static void 
wfd_syslog_logger_log(
    void * user_data,
    int level,
    char const * format,
    va_list args)
{
    (void) user_data;

    int prio = wfd_syslog_logger_to_priority(level);
    vsyslog(prio, format, args);
}

static void
wfd_syslog_logger_close(
    void * user_data)
{
    closelog();
}

void
wfd_syslog_logger_init(
    int level,
    char const * ident,
    int options,
    int facility)
{
    wfd_logger_init(level, 
        &wfd_syslog_logger_log, 
        &wfd_syslog_logger_close, 
        NULL);

    openlog(ident, options, facility);
}