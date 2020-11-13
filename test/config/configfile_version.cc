#include <gtest/gtest.h>

#include "webfused/config/config.h"
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

TEST(configfile_version, invalid_major_version_too_low)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);

    char const too_low[] = "version = { major = 0, minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n";

    struct wfd_config * config= wfd_config_load_string(too_low);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_version, invalid_major_version_too_high)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);

    char const too_high[] = "version = { major = 99, minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_version, invalid_missing_major_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);

    char const too_high[] = "version = { minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_version, invalid_missing_minor_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);

    char const too_high[] = "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR " }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_version, valid_older_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_INFO, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));

    char const valid[] = "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = -1 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_version, valid_newer_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));

    char const valid[] = "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = 99 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);
}
