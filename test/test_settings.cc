#include <gtest/gtest.h>
#include "webfused/config/settings_intern.h"
#include "webfused/config/settings.h"
#include <libconfig.h>

TEST(settings, settings)
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
    ASSERT_STREQ("some.string", wfd_settings_get(&settings, "string_value"));
    ASSERT_EQ(nullptr, wfd_settings_get(&settings, "int_value"));
    ASSERT_EQ(nullptr, wfd_settings_get(&settings, "invalid_value"));

    wfd_settings_cleanup(&settings);
    config_destroy(&config);
}