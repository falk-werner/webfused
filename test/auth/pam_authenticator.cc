#include "webfused/auth/pam_authenticator.h"
#include "webfused/auth/authenticator.h"
#include "webfused/config/settings.h"
#include "webfused/auth/factory.h"

#include "mock/credentials.hpp"
#include "mock/settings.hpp"
#include "mock/pam.hpp"

#include <gtest/gtest.h>


using ::webfused_test::MockSettings;
using ::webfused_test::MockCredentials;
using ::webfused_test::MockPam;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::Invoke;
using ::testing::StrictMock;

TEST(pam_authenticator, create)
{
    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, create_via_factory)
{
    wfd_authenticator authenticator;
    bool success = wfd_authenticator_create("pam", nullptr, &authenticator);
    ASSERT_TRUE(success);

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, get_type)
{
    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    ASSERT_STREQ("username", wfd_authenticator_get_type(authenticator));

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, authenticate)
{
    MockPam pam;
    EXPECT_CALL(pam, pam_start(StrEq("webfused"), nullptr, _, _)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_authenticate(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_acct_mgmt(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_end(_, _)).Times(1).WillOnce(Return(PAM_SUCCESS));

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_TRUE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, authenticate_with_custom_service_name)
{
    MockPam pam;
    EXPECT_CALL(pam, pam_start(StrEq("brummni"), nullptr, _, _)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_authenticate(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_acct_mgmt(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_end(_, _)).Times(1).WillOnce(Return(PAM_SUCCESS));

    MockSettings settings;
    EXPECT_CALL(settings, wfd_settings_get_string_or_default(_,StrEq("service_name"), StrEq("webfused")))
        .Times(1).WillOnce(Return("brummni"));

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_TRUE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

namespace
{

int valid_conversation(
    char const * service_name,
    char const * user,
    struct pam_conv const * conv,
    pam_handle_t * * handle)
{
    (void) service_name;
    (void) user;
    (void) handle;

    pam_message request_username = {PAM_PROMPT_ECHO_ON, "username"};
    pam_message request_password = {PAM_PROMPT_ECHO_OFF, "password"};
    pam_message const * messages[2] =
    {
        &request_username,
        &request_password
    };
    pam_response * responses;
    int rc = conv->conv(2, messages, &responses, conv->appdata_ptr);
    if (PAM_SUCCESS == rc)
    {
        free(responses[0].resp);
        free(responses[1].resp);
        free(responses);
    }

    return rc;
}

}

TEST(pam_authenticator, conversation_with_valid_messages)
{
    MockPam pam;
    EXPECT_CALL(pam, pam_start(StrEq("webfused"), nullptr, _, _))
        .Times(1).WillOnce(Invoke(&valid_conversation));
    EXPECT_CALL(pam, pam_authenticate(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_acct_mgmt(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_end(_, _)).Times(1).WillOnce(Return(PAM_SUCCESS));

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_TRUE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

namespace
{

int invalid_conversation(
    char const * service_name,
    char const * user,
    struct pam_conv const * conv,
    pam_handle_t * * handle)
{
    (void) service_name;
    (void) user;
    (void) handle;

    pam_message invalid_request = {-1, "invalid"};
    pam_message const * messages[2] =
    {
        &invalid_request
    };
    pam_response * responses;
    int rc = conv->conv(1, messages, &responses, conv->appdata_ptr);

    return rc;
}

}

TEST(pam_authenticator, conversation_with_invalid_messages)
{
    MockPam pam;
    EXPECT_CALL(pam, pam_start(StrEq("webfused"), nullptr, _, _))
        .Times(1).WillOnce(Invoke(&invalid_conversation));

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, authenticate_fail_authenticate)
{
    MockPam pam;
    EXPECT_CALL(pam, pam_start(StrEq("webfused"), nullptr, _, _)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_authenticate(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(-1));
    EXPECT_CALL(pam, pam_end(_, _)).Times(1).WillOnce(Return(PAM_SUCCESS));

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, authenticate_fail_acct_mgmt)
{
    MockPam pam;
    EXPECT_CALL(pam, pam_start(StrEq("webfused"), nullptr, _, _)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_authenticate(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(PAM_SUCCESS));
    EXPECT_CALL(pam, pam_acct_mgmt(_, PAM_DISALLOW_NULL_AUTHTOK)).Times(1).WillOnce(Return(-1));
    EXPECT_CALL(pam, pam_end(_, _)).Times(1).WillOnce(Return(PAM_SUCCESS));

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, authenticate_fail_missing_username)
{
    StrictMock<MockPam> pam;

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return(nullptr));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return("secret"));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}

TEST(pam_authenticator, authenticate_fail_missing_password)
{
    StrictMock<MockPam> pam;

    wfd_authenticator authenticator;
    bool success = wfd_pam_authenticator_create(nullptr, &authenticator);
    ASSERT_TRUE(success);

    MockCredentials creds;
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("username"))).Times(1).WillOnce(Return("bob"));
    EXPECT_CALL(creds, wf_credentials_get(_,StrEq("password"))).Times(1).WillOnce(Return(nullptr));

    bool is_authenticated = wfd_authenticator_authenticate(authenticator, nullptr);
    ASSERT_FALSE(is_authenticated);

    wfd_authenticator_dispose(authenticator);
}
