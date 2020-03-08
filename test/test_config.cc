#include <gtest/gtest.h>

#include "webfused/config/factory.h"
#include "webfused/config/config.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "mock_logger.hpp"


using ::testing::_;
using ::webfused_test::MockLogger;

TEST(config, is_loadable)
{
    struct wfd_config * config = wfd_config_load_file("webfused.conf");
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}

TEST(config, minimal_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const minimal[] = "version = { major = 1, minor = 0 }\n";
    struct wfd_config * config = wfd_config_load_string(minimal);
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}

TEST(config, invalid_major_version_too_low)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const too_low[] = "version = { major = 0, minor = 0 }\n";

    struct wfd_config * config = wfd_config_load_string(too_low);
    ASSERT_EQ(nullptr, config);    
}

TEST(config, invalid_major_version_too_high)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const too_high[] = "version = { major = 2, minor = 0 }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);    
}

TEST(config, invalid_missing_major_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const too_high[] = "version = { minor = 0 }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);    
}

TEST(config, invalid_missing_minor_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const too_high[] = "version = { major = 1 }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);    
}

TEST(config, valid_older_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_INFO, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);
    
    char const valid[] = "version = { major = 1, minor = -1 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}

TEST(config, valid_newer_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const valid[] = "version = { major = 1, minor = 1 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);

    wfd_config_dispose(config);
}