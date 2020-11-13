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

TEST(configfile_server, vhost_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_set_server_vhostname(_,StrEq("some.host"))).Times(1);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "server:\n"
        "{\n"
        "  vhost_name = \"some.host\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_server, port)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_set_server_port(_,54321)).Times(1);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "server:\n"
        "{\n"
        "  port = 54321\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_server, tls_certificate)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_set_server_cert(_, StrEq("/path/to/cert.pem"))).Times(1);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "server:\n"
        "{\n"
        "  tls:\n"
        "  {\n"
        "    certificate = \"/path/to/cert.pem\"\n"
        "  }\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_server, tls_key)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_set_server_key(_,StrEq("/path/to/key.pem"))).Times(1);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "server:\n"
        "{\n"
        "  tls:\n"
        "  {\n"
        "    key = \"/path/to/key.pem\"\n"
        "  }\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_server, document_root)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_set_server_document_root(_,StrEq("/var/www"))).Times(1);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "server:\n"
        "{\n"
        "  document_root = \"/var/www\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}
