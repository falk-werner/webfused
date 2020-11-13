#include <gtest/gtest.h>
#include "webfused/config/config.h"
#include "webfused/config/config_intern.h"
#include "mock/settings.hpp"

#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "webfused/util/string_list.h"
#include "mock/logger.hpp"

using ::webfused_test::MockLogger;
using ::testing::_;

using ::webfused_test::MockSettings;
using ::testing::Return;
using ::testing::StrEq;

TEST(config, server_config)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_set_server_vhostname(config, "localhost");
    wfd_config_set_server_port(config, 8443);
    wfd_config_set_server_key(config, "/path/to/key.pem");
    wfd_config_set_server_cert(config, "/path/to/cert.pem");
    wfd_config_set_server_document_root(config, "/var/www");

    wf_server_config * server_config = wfd_config_get_server_config(config);
    ASSERT_NE(nullptr, server_config);

    wfd_config_dispose(config);
}

TEST(config, auth_config)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("/any/path"));

    bool success = wfd_config_add_auth_provider(config, "file", nullptr);
    ASSERT_TRUE(success);

    wfd_config_dispose(config);
}

TEST(config, auth_config_failed_to_add_second_provider)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("/any/path"));

    bool success = wfd_config_add_auth_provider(config, "file", nullptr);
    ASSERT_TRUE(success);

    success = wfd_config_add_auth_provider(config, "file", nullptr);
    ASSERT_FALSE(success);

    wfd_config_dispose(config);
}

TEST(config, auth_config_failed_to_add_unknown_provider)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    bool success = wfd_config_add_auth_provider(config, "unknown", nullptr);
    ASSERT_FALSE(success);

    wfd_config_dispose(config);
}

TEST(config, add_filesystem)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    struct wfd_string_list mount_options = {0, 0, nullptr};
    bool success = wfd_config_add_filesystem(config, "test", "/tmp/test", &mount_options);
    ASSERT_TRUE(success);

    wfd_config_dispose(config);
}

TEST(config, set_logger)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    bool success = wfd_config_set_logger(config, "stderr", WFD_LOGLEVEL_ALL, nullptr);
    ASSERT_TRUE(success);

    wfd_config_dispose(config);
}

TEST(config, do_set_user)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_set_user(config, "some.user", "some.group");
    ASSERT_STREQ("some.user", wfd_config_get_user(config));
    ASSERT_STREQ("some.group", wfd_config_get_group(config));

    wfd_config_dispose(config);
}
