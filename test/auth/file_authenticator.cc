#include "webfused/auth/file_authenticator.h"
#include "webfused/auth/authenticator.h"
#include "webfused/config/settings.h"
#include "webfused/auth/factory.h"

#include "mock/credentials.hpp"
#include "mock/settings.hpp"

#include <gtest/gtest.h>
#include <libconfig.h>

using ::webfused_test::MockSettings;
using ::webfused_test::MockCredentials;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;

TEST(file_authenticator, create)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("test_passwd.json"));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    wfd_authenticator_dispose(authenticator);
}

TEST(file_authenticator, create_fail_missing_file)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return(nullptr));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_FALSE(success);
}

TEST(file_authenticator, create_via_factory)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("test_passwd.json"));

    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create("file", nullptr, &authenticator);
    ASSERT_TRUE(success);

    wfd_authenticator_dispose(authenticator);
}

TEST(file_authenticator, authenticate)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("test_passwd.json"));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_TRUE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(file_authenticator, authenticate_fail_wrong_passwd)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("test_passwd.json"));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("unkown"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(file_authenticator, authenticate_fail_no_passwd_file)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("unknown_passwd.json"));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("secred"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(file_authenticator, authenticate_fail_missing_username)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("test_passwd.json"));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return(nullptr));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return("unkown"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(file_authenticator, authenticate_fail_missing_password)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("test_passwd.json"));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, get(StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, get(StrEq("password"))).Times(1).WillOnce(Return(nullptr));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(file_authenticator, get_type)
{
    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string(_,StrEq("file"))).Times(1).WillOnce(Return("/any/path"));

    wfd_authenticator authenticator;
    bool success = wfd_file_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    ASSERT_STREQ("username", wfd_authenticator_get_type(authenticator));

    wfd_authenticator_dispose(authenticator);
}
