#include <gtest/gtest.h>

#include "webfused/config/factory.h"
#include "webfused/config/config.h"

TEST(config, is_loadable)
{
    struct wfd_config * config = wfd_config_load_file("webfused.conf");
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}

TEST(config, minimal_config)
{
    char const minimal[] = "version = { major = 1, minor = 0 }\n";
    struct wfd_config * config = wfd_config_load_string(minimal);
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}

TEST(config, invalid_major_version_too_low)
{
    char const too_low[] = "version = { major = 0, minor = 0 }\n";

    struct wfd_config * config = wfd_config_load_string(too_low);
    ASSERT_EQ(nullptr, config);    
}

TEST(config, invalid_major_version_too_high)
{
    char const too_high[] = "version = { major = 2, minor = 0 }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);    
}

TEST(config, valid_older_minor)
{
    char const valid[] = "version = { major = 1, minor = -1 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}

TEST(config, valid_newer_minor)
{
    char const valid[] = "version = { major = 1, minor = 1 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}