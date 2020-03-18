#include <gtest/gtest.h>
#include "webfused/config/config.h"
#include "mock_settings.hpp"

#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "mock_logger.hpp"
using ::webfused_test::MockLogger;
using ::testing::_;

using ::webfused_test::MockSettings;
using ::testing::Return;
using ::testing::StrEq;

TEST(config, server_config)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_builder builder = wfd_config_get_builder(config);
    wfd_config_builder_set_server_vhostname(builder, "localhost");
    wfd_config_builder_set_server_port(builder, 8443);
    wfd_config_builder_set_server_key(builder, "/path/to/key.pem");
    wfd_config_builder_set_server_cert(builder, "/path/to/cert.pem");
    wfd_config_builder_set_server_document_root(builder, "/var/www");

    wf_server_config * server_config = wfd_config_get_server_config(config);
    ASSERT_NE(nullptr, server_config);

    wfd_config_dispose(config);
}

TEST(config, auth_config)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_builder builder = wfd_config_get_builder(config);

    MockSettings settings;
    EXPECT_CALL(settings, get(StrEq("file"))).Times(1).WillOnce(Return("/any/path"));

    bool success = wfd_config_builder_add_auth_provider(builder, "file", nullptr);
    ASSERT_TRUE(success);

    wfd_config_dispose(config);
}

TEST(config, auth_config_failed_to_add_second_provider)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_builder builder = wfd_config_get_builder(config);

    MockSettings settings;
    EXPECT_CALL(settings, get(StrEq("file"))).Times(1).WillOnce(Return("/any/path"));

    bool success = wfd_config_builder_add_auth_provider(builder, "file", nullptr);
    ASSERT_TRUE(success);

    success = wfd_config_builder_add_auth_provider(builder, "file", nullptr);
    ASSERT_FALSE(success);

    wfd_config_dispose(config);
}

TEST(config, auth_config_failed_to_add_unknown_provider)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_builder builder = wfd_config_get_builder(config);

    bool success = wfd_config_builder_add_auth_provider(builder, "unknown", nullptr);
    ASSERT_FALSE(success);

    wfd_config_dispose(config);
}

TEST(config, add_filesystem)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_builder builder = wfd_config_get_builder(config);

    bool success = wfd_config_builder_add_filesystem(builder, "test", "/tmp/test");
    ASSERT_TRUE(success);

    wfd_config_dispose(config);
}