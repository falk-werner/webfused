#include "webfused/log/syslog_logger.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "webfused/config/settings.h"

#include <syslog.h>
#include <strings.h>

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

struct wfd_syslog_facility
{
    char const * name;
    int value;
};

static bool
wfd_syslog_logger_parse_facility(
    char const * facility,
    int * result)
{
    static struct wfd_syslog_facility const facilities[] =
    {
        {"auth", LOG_AUTH},
        {"authpriv", LOG_AUTHPRIV},
        {"cron", LOG_CRON},
        {"daemon", LOG_DAEMON},
        {"fpt", LOG_FTP},
        {"kern", LOG_KERN},
        {"local0", LOG_LOCAL0},
        {"local1", LOG_LOCAL1},
        {"local2", LOG_LOCAL2},
        {"local3", LOG_LOCAL3},
        {"local4", LOG_LOCAL4},
        {"local5", LOG_LOCAL5},
        {"local6", LOG_LOCAL6},
        {"local7", LOG_LOCAL7},
        {"lpr", LOG_LPR},
        {"mail", LOG_MAIL},
        {"news", LOG_NEWS},
        {"syslog", LOG_SYSLOG},
        {"user", LOG_USER},
        {"uucp", LOG_UUCP}
    };
    static size_t const facilites_count = sizeof(facilities) / sizeof(facilities[0]);

    for (size_t i = 0; i < facilites_count; i++)
    {
        if (0 == strcasecmp(facility, facilities[i].name))
        {
            *result = facilities[i].value;
            return true;
        }
    }

    return false;
}


bool
wfd_syslog_logger_init(
    int level,
    struct wfd_settings * settings)
{
    char const * ident = wfd_settings_get_string_or_default(settings, "ident", "webfused");
    char const * facility_str = wfd_settings_get_string_or_default(settings, "facility", "daemon");
    bool log_pid = wfd_settings_get_bool(settings, "log_pid");

    int facility;
    bool result = wfd_syslog_logger_parse_facility(facility_str, &facility);
    if (result)
    {
        int options = (log_pid) ? LOG_PID : 0;
        wfd_logger_init(level, 
            &wfd_syslog_logger_log, 
            &wfd_syslog_logger_close, 
            NULL);

        openlog(ident, options, facility);
    }
    else
    {
        WFD_ERROR("failed to init syslog logger: invalid log facility: \'%s\'", facility_str);        
    }

    return result;
}