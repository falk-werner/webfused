#include "webfused/log/stderr_logger.h"
#include "webfused/log/manager.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"

#include <gtest/gtest.h>
#include <stddef.h>

TEST(stderr_logger, init)
{
    ASSERT_TRUE(wfd_stderr_logger_init(WFD_LOGLEVEL_ALL, NULL));
    wfd_logger_close();
}

TEST(stderr_logger, init_via_manager)
{
    ASSERT_TRUE(wfd_log_manager_set_logger("stderr", WFD_LOGLEVEL_ALL, NULL));
    wfd_logger_close();
}

TEST(stderr_logger, log)
{
    ASSERT_TRUE(wfd_stderr_logger_init(WFD_LOGLEVEL_ALL, NULL));

    WFD_FATAL("webfused test");
    WFD_ERROR("webfused test");
    WFD_WARN ("webfused test");
    WFD_INFO ("webfused test");
    WFD_DEBUG("webfused test");
    wfd_log(-1, "webfused test");

    wfd_logger_close();
}