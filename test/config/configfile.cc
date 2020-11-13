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

TEST(configfile, is_loadable)
{
    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_set_logger(_,_,_,_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(builder, wfd_config_set_server_vhostname(_,StrEq("localhost"))).Times(1);
    EXPECT_CALL(builder, wfd_config_set_server_port(_,8080)).Times(1);
    EXPECT_CALL(builder, wfd_config_add_auth_provider(_,_, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(1).WillOnce(Return(true));

    struct wfd_config * config = wfd_config_load_file("webfused.conf");
    ASSERT_NE(nullptr, config);
}

TEST(configfile, minimal_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));

    char const minimal[] = "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n";
    struct wfd_config * config = wfd_config_load_string(minimal);
    ASSERT_NE(nullptr, config);
}

TEST(configfile, invalid_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(0);
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(0);

    char const syntax_error[] = "version.major = " WFD_CONFIG_VERSION_STR_MAJOR "\n";

    wfd_config * config = wfd_config_load_string(syntax_error);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile, invalid_config_file)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(0);
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(0);

    struct wfd_config * config = wfd_config_load_file("invalid.conf");
    ASSERT_EQ(nullptr, config);
}
