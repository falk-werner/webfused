#include "webfused/log/syslog_logger.h"
#include "webfused/log/manager.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"

#include "mock/settings.hpp"

#include <gtest/gtest.h>
#include <stddef.h>

using ::webfused_test::MockSettings;
using ::testing::StrEq;
using ::testing::_;
using ::testing::Return;

TEST(syslog_logger, init)
{
    ASSERT_TRUE(wfd_syslog_logger_init(WFD_LOGLEVEL_ALL, NULL));
    wfd_logger_close();
}

TEST(syslog_logger, init_fail_invalid_facility)
{
    MockSettings settings;
    EXPECT_CALL(settings, getStringOrDefault(StrEq("ident"), _)).Times(1).WillOnce(Return("webfused_test"));
    EXPECT_CALL(settings, getStringOrDefault(StrEq("facility"), _)).Times(1).WillOnce(Return("invalid"));
    EXPECT_CALL(settings, getBool(StrEq("log_pid"))).Times(1).WillOnce(Return(false));

    wfd_settings * fake_settings = reinterpret_cast<wfd_settings*>(0xDEADBEEF);
    ASSERT_FALSE(wfd_syslog_logger_init(WFD_LOGLEVEL_ALL, fake_settings));
}

TEST(syslog_logger, init_via_manager)
{
    ASSERT_TRUE(wfd_log_manager_set_logger("syslog", WFD_LOGLEVEL_ALL, NULL));
    wfd_logger_close();
}

TEST(syslog_logger, log)
{
    ASSERT_TRUE(wfd_syslog_logger_init(WFD_LOGLEVEL_ALL, NULL));

    WFD_FATAL("webfused test");
    WFD_ERROR("webfused test");
    WFD_WARN ("webfused test");
    WFD_INFO ("webfused test");
    WFD_DEBUG("webfused test");
    wfd_log(-1, "webfused test");

    wfd_logger_close();
}