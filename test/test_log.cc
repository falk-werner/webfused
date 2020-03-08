#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "mock_logger.hpp"

using ::testing::_;
using ::testing::StrEq;
using ::webfused_test::MockLogger;

TEST(log, fatal)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_FATAL, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &wfd_MockLogger_log, &wfd_MockLogger_onclose, logger.getUserData());
    WFD_FATAL("too bad");
    wfd_logger_close();
}

TEST(log, error)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &wfd_MockLogger_log, &wfd_MockLogger_onclose, logger.getUserData());
    WFD_ERROR("too bad");
    wfd_logger_close();
}

TEST(log, warn)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &wfd_MockLogger_log, &wfd_MockLogger_onclose, logger.getUserData());
    WFD_WARN("too bad");
    wfd_logger_close();
}

TEST(log, info)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_INFO, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &wfd_MockLogger_log, &wfd_MockLogger_onclose, logger.getUserData());
    WFD_INFO("too bad");
    wfd_logger_close();
}

TEST(log, debug)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_DEBUG, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &wfd_MockLogger_log, &wfd_MockLogger_onclose, logger.getUserData());
    WFD_DEBUG("too bad");
    wfd_logger_close();
}

TEST(log, respect_loglevel)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_WARN, &wfd_MockLogger_log, &wfd_MockLogger_onclose, logger.getUserData());
    WFD_DEBUG("too bad");
    wfd_logger_close();
}

TEST(log, log_same_loglevel)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_WARN, &wfd_MockLogger_log, &wfd_MockLogger_onclose, logger.getUserData());
    WFD_WARN("too bad");
    wfd_logger_close();
}

TEST(log, omit_onclose_if_nullptr)
{
    MockLogger logger(true);

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(0);

    wfd_logger_init(WFD_LOGLEVEL_WARN, &wfd_MockLogger_log, nullptr, logger.getUserData());
    WFD_WARN("too bad");
    wfd_logger_close();
}
