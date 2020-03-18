#include "webfused/log/logger.h"
#include "webfused/log/log.h"

#include <stddef.h>

struct wfd_logger
{
    int level;
    wfd_logger_log_fn * log;
    wfd_logger_onclose_fn * onclose;
    void * user_data;
};

static void wfd_logger_default_log(
    void * user_data,
    int level,
    char const * format,
    va_list args);

static struct wfd_logger g_wfd_logger =
{
    .level = WFD_LOGLEVEL_ALL,
    .log = &wfd_logger_default_log,
    .onclose = NULL,
    .user_data = NULL
};

void
wfd_logger_init(
    int level,
    wfd_logger_log_fn * log,
    wfd_logger_onclose_fn * onclose,
    void * user_data)
{
    wfd_logger_close();

    g_wfd_logger.level = level;
    g_wfd_logger.log = log;
    g_wfd_logger.onclose = onclose;
    g_wfd_logger.user_data = user_data;
}

void
wfd_logger_close(void)
{
    if (NULL != g_wfd_logger.onclose)
    {
        g_wfd_logger.onclose(g_wfd_logger.user_data);
    }

    g_wfd_logger.level = WFD_LOGLEVEL_ALL;
    g_wfd_logger.log = &wfd_logger_default_log;
    g_wfd_logger.onclose = NULL;
    g_wfd_logger.user_data = NULL;
}

void
wfd_log(
    int level,
    char const * format,
    ...)
{
    if (g_wfd_logger.level >= level)
    {
        va_list args;
        va_start(args, format);
        g_wfd_logger.log(
            g_wfd_logger.user_data,
            level,
            format,
            args);
        va_end(args);
    }
}

static void wfd_logger_default_log(
    void * user_data,
    int level,
    char const * format,
    va_list args)
{
    (void) user_data;
    (void) level;
    (void) format;
    (void) args;
}
