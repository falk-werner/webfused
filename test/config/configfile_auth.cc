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

TEST(config_auth, authentication)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_add_auth_provider(_,_, _)).Times(1).WillOnce(Return(true));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config_auth, failed_create_authenticator)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_add_auth_provider(_,_, _)).Times(1).WillOnce(Return(false));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config_auth, failed_missing_auth_provider)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config_auth, failed_missing_auth_settings)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "  }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config_auth, failed_auth_settings_get_elem)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);

    MockLibConfig libconfig;
    EXPECT_CALL(libconfig, config_setting_get_elem(_,_)).Times(1).WillOnce(Return(nullptr));

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}
