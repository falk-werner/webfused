#include <gtest/gtest.h>
#include <libconfig.h>

TEST(config, is_loadable)
{
    config_t config;
    config_init(&config);

    int result = config_read_file(&config, "webfused.conf");
    ASSERT_EQ(CONFIG_TRUE, result);

    config_destroy(&config);
}