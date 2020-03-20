#include "webfused/change_user.h"
#include "mock/linux.hpp"
#include <gtest/gtest.h>

using ::webfused_test::MockLinux;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::StrEq;

TEST(change_user, nop_id_not_root)
{
    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(42));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_TRUE(success);
}

TEST(change_user, change_successfully)
{
    struct group group;
    group.gr_gid = 23;

    struct passwd userinfo;
    userinfo.pw_uid = 42;

    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(&group));
    EXPECT_CALL(sys, setgid(23)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, setgroups(0, nullptr)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getpwnam("webfused")).Times(1).WillOnce(Return(&userinfo));
    EXPECT_CALL(sys, setuid(42)).Times(1).WillOnce(Return(0));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_TRUE(success);
}

TEST(change_user, fail_no_username_or_password)
{
    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).WillRepeatedly(Return(0));

    ASSERT_FALSE(wfd_change_user(nullptr, "daemons"));
    ASSERT_FALSE(wfd_change_user("webfused", nullptr));
    ASSERT_FALSE(wfd_change_user(nullptr, nullptr));
}

TEST(change_user, fail_setuid)
{
    struct group group;
    group.gr_gid = 23;

    struct passwd userinfo;
    userinfo.pw_uid = 42;

    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(&group));
    EXPECT_CALL(sys, setgid(23)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, setgroups(0, nullptr)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getpwnam("webfused")).Times(1).WillOnce(Return(&userinfo));
    EXPECT_CALL(sys, setuid(42)).Times(1).WillOnce(Return(-1));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_FALSE(success);
}

TEST(change_user, fail_getpwnam)
{
    struct group group;
    group.gr_gid = 23;

    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(&group));
    EXPECT_CALL(sys, setgid(23)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, setgroups(0, nullptr)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getpwnam("webfused")).Times(1).WillOnce(Return(nullptr));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_FALSE(success);
}

TEST(change_user, fail_setgroups)
{
    struct group group;
    group.gr_gid = 23;

    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(&group));
    EXPECT_CALL(sys, setgid(23)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, setgroups(0, nullptr)).Times(1).WillOnce(Return(-1));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_FALSE(success);
}

TEST(change_user, fail_setgid)
{
    struct group group;
    group.gr_gid = 23;

    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(&group));
    EXPECT_CALL(sys, setgid(23)).Times(1).WillOnce(Return(-1));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_FALSE(success);
}

TEST(change_user, fail_getgrpnam)
{
    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(nullptr));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_FALSE(success);
}

TEST(change_user, fail_switch_to_root)
{
    struct group group;
    group.gr_gid = 23;

    struct passwd userinfo;
    userinfo.pw_uid = 0;

    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(&group));
    EXPECT_CALL(sys, setgid(23)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, setgroups(0, nullptr)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getpwnam("webfused")).Times(1).WillOnce(Return(&userinfo));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_FALSE(success);
}

TEST(change_user, fail_switch_to_root_group)
{
    struct group group;
    group.gr_gid = 0;

    StrictMock<MockLinux> sys;
    EXPECT_CALL(sys, getuid).Times(1).WillOnce(Return(0));
    EXPECT_CALL(sys, getgrnam(StrEq("daemons"))).Times(1).WillOnce(Return(&group));

    bool success = wfd_change_user("webfused", "daemons");
    ASSERT_FALSE(success);
}
