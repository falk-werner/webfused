#include "webfused/log/log.h"
#include <strings.h>

char const *
wfd_log_level_tostring(int level)
{
    switch (level)
    {
        case WFD_LOGLEVEL_NONE: return "none";
        case WFD_LOGLEVEL_FATAL: return "fatal";
        case WFD_LOGLEVEL_ERROR: return "error";
        case WFD_LOGLEVEL_WARN: return "warn";
        case WFD_LOGLEVEL_INFO: return "info";
        case WFD_LOGLEVEL_DEBUG: return "debug";
        case WFD_LOGLEVEL_ALL: return "all";
        default: return "<unknown>";
    }
}

bool
wfd_log_level_parse(
    char const * level,
    int * result)
{
    bool success = true;

    if (0 == strcasecmp("all", level))
    {
        *result = WFD_LOGLEVEL_ALL;
    }
    else if (0 == strcasecmp("none", level))
    {
        *result = WFD_LOGLEVEL_NONE;
    }
    else if (0 == strcasecmp("fatal", level))
    {
        *result = WFD_LOGLEVEL_FATAL;
    }
    else if (0 == strcasecmp("error", level))
    {
        *result = WFD_LOGLEVEL_ERROR;
    }
    else if ((0 == strcasecmp("warn", level)) ||
             (0 == strcasecmp("warning", level)))
    {
        *result = WFD_LOGLEVEL_WARN;
    }
    else if (0 == strcasecmp("info", level))
    {
        *result = WFD_LOGLEVEL_INFO;
    }
    else if (0 == strcasecmp("debug", level))
    {
        *result = WFD_LOGLEVEL_DEBUG;
    }
    else
    {
        return false;
    }

    return success;
}
