#include <gtest/gtest.h>
#include "webfused/mountpoint_factory.h"
#include <webfuse/adapter/mountpoint.h>

#include <cstring>

TEST(mountpoint_factory, create)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, add_filesystem)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    bool success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/tmp/webfused_test");
    ASSERT_TRUE(success);

    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, add_filesystem_fail_to_add_twice)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    bool success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/tmp/webfused_test");
    ASSERT_TRUE(success);

    success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/tmp/webfused_test");
    ASSERT_FALSE(success);

    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, add_filesystem_multi)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);


    for (size_t i = 0; i < 24; i++)
    {
        char name[10];
        snprintf(name, 10, "test_%zu", i);
        bool success = wfd_mountpoint_factory_add_filesystem(factory, name, "/tmp/webfused_test");
        ASSERT_TRUE(success) << i;
    }

    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, add_filesystem_fail_invalid_path)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    bool success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/do/not/allow/nested/paths");
    ASSERT_FALSE(success);

    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, create_mountpoint)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    bool success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/tmp/webfuse_test");
    ASSERT_TRUE(success);

    wf_mountpoint * mountpoint = wfd_mountpoint_factory_create_mountpoint("test", factory);
    ASSERT_NE(nullptr, mountpoint);
    ASSERT_STREQ("/tmp/webfuse_test", wf_mountpoint_get_path(mountpoint));

    wf_mountpoint_dispose(mountpoint);
    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, create_mountpoint_fail_already_in_use)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    bool success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/tmp/webfuse_test");
    ASSERT_TRUE(success);

    wf_mountpoint * mountpoint = wfd_mountpoint_factory_create_mountpoint("test", factory);
    ASSERT_NE(nullptr, mountpoint);
    ASSERT_STREQ("/tmp/webfuse_test", wf_mountpoint_get_path(mountpoint));

    wf_mountpoint * mountpoint2 = wfd_mountpoint_factory_create_mountpoint("test", factory);
    ASSERT_EQ(nullptr, mountpoint2);

    wf_mountpoint_dispose(mountpoint);
    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, create_mountpoint_fail_unknown_filesystem)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    bool success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/tmp/webfuse_test");
    ASSERT_TRUE(success);

    wf_mountpoint * mountpoint = wfd_mountpoint_factory_create_mountpoint("unkown", factory);
    ASSERT_EQ(nullptr, mountpoint);

    wfd_mountpoint_factory_dispose(factory);
}

TEST(mountpiont_factory, create_mountpoint_multi)
{
    wfd_mountpoint_factory * factory = wfd_mountpoint_factory_create();
    ASSERT_NE(nullptr, factory);

    bool success = wfd_mountpoint_factory_add_filesystem(factory, "test", "/tmp/webfuse_test");
    ASSERT_TRUE(success);

    for(int i = 0; i < 5; i++)
    {
        wf_mountpoint * mountpoint = wfd_mountpoint_factory_create_mountpoint("test", factory);
        ASSERT_NE(nullptr, mountpoint) << i;
        ASSERT_STREQ("/tmp/webfuse_test", wf_mountpoint_get_path(mountpoint)) << i;

        wf_mountpoint_dispose(mountpoint);
    }
 
    wfd_mountpoint_factory_dispose(factory);
}
