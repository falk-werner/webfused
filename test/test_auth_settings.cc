#include <gtest/gtest.h>
#include "webfused/auth/settings.h"
#include "webfused/config/auth_settings.h"
#include <libconfig.h>

TEST(auth_settings, auth_settings)
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

    config_setting_t * settings = config_lookup(&config, "settings");
    ASSERT_NE(nullptr, settings);

    wfd_auth_settings * auth_settings = wfd_auth_settings_create("test_provider", settings);
    ASSERT_STREQ("test_provider", wfd_auth_settings_get_provider(auth_settings));
    ASSERT_STREQ("some.string", wfd_auth_settings_get(auth_settings, "string_value"));
    ASSERT_EQ(nullptr, wfd_auth_settings_get(auth_settings, "int_value"));
    ASSERT_EQ(nullptr, wfd_auth_settings_get(auth_settings, "invalid_value"));

    wfd_auth_settings_dispose(auth_settings);
    config_destroy(&config);
}