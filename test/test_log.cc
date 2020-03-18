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

TEST(log, default_log)
{
    WFD_ERROR("trigger log");
}

TEST(log, loglevel_parse)
{
    int level;

    ASSERT_TRUE(wfd_log_level_parse("fatal", &level));
    ASSERT_EQ(WFD_LOGLEVEL_FATAL, level);
    ASSERT_TRUE(wfd_log_level_parse("FATAL", &level));
    ASSERT_EQ(WFD_LOGLEVEL_FATAL, level);

    ASSERT_TRUE(wfd_log_level_parse("error", &level));
    ASSERT_EQ(WFD_LOGLEVEL_ERROR, level);
    ASSERT_TRUE(wfd_log_level_parse("ERROR", &level));
    ASSERT_EQ(WFD_LOGLEVEL_ERROR, level);

    ASSERT_TRUE(wfd_log_level_parse("warn", &level));
    ASSERT_EQ(WFD_LOGLEVEL_WARN, level);
    ASSERT_TRUE(wfd_log_level_parse("WARN", &level));
    ASSERT_EQ(WFD_LOGLEVEL_WARN, level);
    ASSERT_TRUE(wfd_log_level_parse("warning", &level));
    ASSERT_EQ(WFD_LOGLEVEL_WARN, level);
    ASSERT_TRUE(wfd_log_level_parse("WARNING", &level));
    ASSERT_EQ(WFD_LOGLEVEL_WARN, level);

    ASSERT_TRUE(wfd_log_level_parse("info", &level));
    ASSERT_EQ(WFD_LOGLEVEL_INFO, level);
    ASSERT_TRUE(wfd_log_level_parse("INFO", &level));
    ASSERT_EQ(WFD_LOGLEVEL_INFO, level);

    ASSERT_TRUE(wfd_log_level_parse("debug", &level));
    ASSERT_EQ(WFD_LOGLEVEL_DEBUG, level);
    ASSERT_TRUE(wfd_log_level_parse("DEBUG", &level));
    ASSERT_EQ(WFD_LOGLEVEL_DEBUG, level);

    ASSERT_FALSE(wfd_log_level_parse("<invalid>", &level));

    ASSERT_TRUE(wfd_log_level_parse("all", &level));
    ASSERT_EQ(WFD_LOGLEVEL_ALL, level);
    ASSERT_TRUE(wfd_log_level_parse("ALL", &level));
    ASSERT_EQ(WFD_LOGLEVEL_ALL, level);

    ASSERT_TRUE(wfd_log_level_parse("none", &level));
    ASSERT_EQ(WFD_LOGLEVEL_NONE, level);
    ASSERT_TRUE(wfd_log_level_parse("NONE", &level));
    ASSERT_EQ(WFD_LOGLEVEL_NONE, level);

}

TEST(log, log_level_tostring)
{
    ASSERT_STREQ("none", wfd_log_level_tostring(WFD_LOGLEVEL_NONE));
    ASSERT_STREQ("fatal", wfd_log_level_tostring(WFD_LOGLEVEL_FATAL));
    ASSERT_STREQ("error", wfd_log_level_tostring(WFD_LOGLEVEL_ERROR));
    ASSERT_STREQ("warn", wfd_log_level_tostring(WFD_LOGLEVEL_WARN));
    ASSERT_STREQ("info", wfd_log_level_tostring(WFD_LOGLEVEL_INFO));
    ASSERT_STREQ("debug", wfd_log_level_tostring(WFD_LOGLEVEL_DEBUG));
    ASSERT_STREQ("all", wfd_log_level_tostring(WFD_LOGLEVEL_ALL));
    ASSERT_STREQ("<unknown>", wfd_log_level_tostring(42));
}