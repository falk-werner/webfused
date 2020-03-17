#include "webfused/auth/file_authenticator.h"
#include "webfused/auth/authenticator.h"
#include "webfused/config/auth_settings.h"
#include "webfused/auth/factory.h"

#include "mock_credentials.hpp"

#include <gtest/gtest.h>
#include <libconfig.h>

using ::webfused_test::MockCredentials;
using ::testing::Return;
using ::testing::StrEq;

TEST(file_authenticator, create)
{
    char const config_text[] =
        "file = \"/tmp/webfuse_passwd.json\"\n"
        ;
    config_t config;
    config_init(&config);
    config_read_string(&config, config_text);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(auth_settings, &authenticator);
    ASSERT_TRUE(success);

    wfd_auth_settings_dispose(auth_settings);
    wfd_authenticator_dispose(authenticator);
    config_destroy(&config);
}

TEST(file_authenticator, create_fail_missing_file)
{
    config_t config;
    config_init(&config);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(auth_settings, &authenticator);
    ASSERT_FALSE(success);

    wfd_auth_settings_dispose(auth_settings);
    config_destroy(&config);
}

TEST(file_authenticator, create_via_factory)
{
    char const config_text[] =
        "file = \"/tmp/webfuse_passwd.json\"\n"
        ;
    config_t config;
    config_init(&config);
    config_read_string(&config, config_text);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create(auth_settings, &authenticator);
    ASSERT_TRUE(success);

    wfd_auth_settings_dispose(auth_settings);
    wfd_authenticator_dispose(authenticator);
    config_destroy(&config);
}

TEST(file_authenticator, authenticate)
{
    char const config_text[] =
        "file = \"test_passwd.json\"\n"
        ;
    config_t config;
    config_init(&config);
    config_read_string(&config, config_text);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create(auth_settings, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_TRUE(is_authenticated);

    wfd_auth_settings_dispose(auth_settings);
    wfd_authenticator_dispose(authenticator);
    config_destroy(&config);
}

TEST(file_authenticator, authenticate_fail_wrong_passwd)
{
    char const config_text[] =
        "file = \"test_passwd.json\"\n"
        ;
    config_t config;
    config_init(&config);
    config_read_string(&config, config_text);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create(auth_settings, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("unkown"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_auth_settings_dispose(auth_settings);
    wfd_authenticator_dispose(authenticator);
    config_destroy(&config);
}

TEST(file_authenticator, authenticate_fail_no_passwd_file)
{
    char const config_text[] =
        "file = \"non_existing_passwd.json\"\n"
        ;
    config_t config;
    config_init(&config);
    config_read_string(&config, config_text);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create(auth_settings, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("secred"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_auth_settings_dispose(auth_settings);
    wfd_authenticator_dispose(authenticator);
    config_destroy(&config);
}

TEST(file_authenticator, authenticate_fail_missing_username)
{
    char const config_text[] =
        "file = \"test_passwd.json\"\n"
        ;
    config_t config;
    config_init(&config);
    config_read_string(&config, config_text);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create(auth_settings, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return(nullptr));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("unkown"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_auth_settings_dispose(auth_settings);
    wfd_authenticator_dispose(authenticator);
    config_destroy(&config);
}

TEST(file_authenticator, authenticate_fail_missing_password)
{
    char const config_text[] =
        "file = \"test_passwd.json\"\n"
        ;
    config_t config;
    config_init(&config);
    config_read_string(&config, config_text);
    config_setting_t * settings = config_root_setting(&config);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("file", settings);

    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create(auth_settings, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return(nullptr));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_auth_settings_dispose(auth_settings);
    wfd_authenticator_dispose(authenticator);
    config_destroy(&config);
}