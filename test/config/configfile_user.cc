#include <gtest/gtest.h>

#include "webfused/config/factory.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "mock/logger.hpp"
#include "mock/config_builder.hpp"
#include "mock/libconfig.hpp"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::StrEq;
using ::webfused_test::MockLogger;
using ::webfused_test::MockConfigBuilder;
using ::webfused_test::MockLibConfig;

TEST(configfile_user, set_user)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_set_user(_, _, _)).Times(1);   

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "user:\n"
        "{\n"
        "  name  = \"webfused\"\n"
        "  group = \"webfused\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_user, set_user_fail_missing_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_set_user(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "user:\n"
        "{\n"
        "  group = \"webfused\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_user, set_user_fail_missing_group)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_set_user(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "user:\n"
        "{\n"
        "  name = \"webfused\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}
