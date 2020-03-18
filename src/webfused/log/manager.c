#include "webfused/log/manager.h"
#include "webfused/log/log.h"
#include "webfused/log/stderr_logger.h"
#include "webfused/log/syslog_logger.h"

#include <string.h>

bool
wfd_log_manager_set_logger(
    char const * provider,
    int level,
    struct wfd_settings * settings)
{
    bool result = false;

    if (0 == strcmp("syslog", provider))
    {
        result = wfd_syslog_logger_init(level, settings);
    }
    else if (0 == strcmp("stderr", provider))
    {
        result = wfd_stderr_logger_init(level, settings);        
    }
    else
    {
        WFD_ERROR("failed to init log: unknown provider \'%s\'", provider);
    }

    return result;
}
