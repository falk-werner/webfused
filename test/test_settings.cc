#include <gtest/gtest.h>
#include "webfused/config/settings_intern.h"
#include "webfused/config/settings.h"
#include <libconfig.h>

TEST(settings, get_string)
{
    char const settings_text[] =
        "settings:\n"
        "{\n"
        "  string_value = \"some.string\"\n"
        "  int_value    = 42\n"
        "}\n"
        ;
    config_t config;
    config_init(&config);
    int rc = config_read_string(&config, settings_text);
    ASSERT_TRUE(CONFIG_TRUE == rc);

    config_setting_t * setting = config_lookup(&config, "settings");
    ASSERT_NE(nullptr, setting);

    wfd_settings settings;
    wfd_settings_init(&settings, setting);
    ASSERT_STREQ("some.string", wfd_settings_get_string(&settings, "string_value"));
    ASSERT_EQ(nullptr, wfd_settings_get_string(&settings, "int_value"));
    ASSERT_EQ(nullptr, wfd_settings_get_string(&settings, "invalid_value"));
    ASSERT_EQ(nullptr, wfd_settings_get_string(NULL, "invalid_value"));

    wfd_settings_cleanup(&settings);
    config_destroy(&config);
}

TEST(settings, get_string_or_default)
{
    char const settings_text[] =
        "settings:\n"
        "{\n"
        "  string_value = \"some.string\"\n"
        "  int_value    = 42\n"
        "}\n"
        ;
    config_t config;
    config_init(&config);
    int rc = config_read_string(&config, settings_text);
    ASSERT_TRUE(CONFIG_TRUE == rc);

    config_setting_t * setting = config_lookup(&config, "settings");
    ASSERT_NE(nullptr, setting);

    wfd_settings settings;
    wfd_settings_init(&settings, setting);
    ASSERT_STREQ("some.string", wfd_settings_get_string_or_default(&settings, "string_value", "default"));
    ASSERT_STREQ("default", wfd_settings_get_string_or_default(&settings, "int_value", "default"));
    ASSERT_STREQ("default", wfd_settings_get_string_or_default(&settings, "invalid_value", "default"));
    ASSERT_STREQ("default", wfd_settings_get_string_or_default(NULL, "invalid_value", "default"));

    wfd_settings_cleanup(&settings);
    config_destroy(&config);
}

TEST(settings, get_bool)
{
    char const settings_text[] =
        "settings:\n"
        "{\n"
        "  true_value   = true\n"
        "  false_value  = false\n"
        "  int_value    = 42\n"
        "}\n"
        ;
    config_t config;
    config_init(&config);
    int rc = config_read_string(&config, settings_text);
    ASSERT_TRUE(CONFIG_TRUE == rc);

    config_setting_t * setting = config_lookup(&config, "settings");
    ASSERT_NE(nullptr, setting);

    wfd_settings settings;
    wfd_settings_init(&settings, setting);
    ASSERT_TRUE(wfd_settings_get_bool(&settings, "true_value"));
    ASSERT_FALSE(wfd_settings_get_bool(&settings, "false_value"));
    ASSERT_FALSE(wfd_settings_get_bool(&settings, "int_value"));
    ASSERT_FALSE(wfd_settings_get_bool(&settings, "invalid_value"));
    ASSERT_FALSE(wfd_settings_get_bool(NULL, "invalid_value"));

    wfd_settings_cleanup(&settings);
    config_destroy(&config);
}