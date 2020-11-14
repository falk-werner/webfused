#include "webfused/util/string_list.h"
#include <gtest/gtest.h>

TEST(string_list, init_cleanup)
{
    wfd_string_list list;
    wfd_string_list_init(&list);

    ASSERT_EQ(0, list.size);

    wfd_string_list_cleanup(&list);
}

TEST(string_list, add)
{
    wfd_string_list list;
    wfd_string_list_init(&list);
    wfd_string_list_add(&list, "value");

    ASSERT_EQ(1, list.size);
    ASSERT_STREQ("value", list.items[0]);

    wfd_string_list_cleanup(&list);
}

TEST(string_list, add_many)
{
    wfd_string_list list;
    wfd_string_list_init(&list);

    constexpr size_t count = 256;
    for (size_t i = 0; i < count; i++)
    {
        wfd_string_list_add(&list, "value");
    }

    ASSERT_EQ(count, list.size);
    for (size_t i = 0; i < count; i++)
    {
        ASSERT_STREQ("value", list.items[i]);
    }

    wfd_string_list_cleanup(&list);
}
