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

TEST(config, invalid_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const syntax_error[] = "version.major = 1\n";

    struct wfd_config * config = wfd_config_load_string(syntax_error);
    ASSERT_EQ(nullptr, config);    
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

TEST(config, default_values)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const minimal[] = "version = { major = 1, minor = 0 }\n";
    struct wfd_config * config = wfd_config_load_string(minimal);
    ASSERT_NE(nullptr, config);

    ASSERT_EQ(8080, wfd_config_get_server_port(config));
    ASSERT_STREQ("localhost", wfd_config_get_server_vhostname(config));
    ASSERT_EQ(nullptr, wfd_config_get_server_cert(config));
    ASSERT_EQ(nullptr, wfd_config_get_server_key(config));
    ASSERT_EQ(nullptr, wfd_config_get_server_document_root(config));

    wfd_config_dispose(config);
}

TEST(config, vhost_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  vhost_name = \"some.host\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);

    ASSERT_STREQ("some.host", wfd_config_get_server_vhostname(config));

    wfd_config_dispose(config);
}

TEST(config, port)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  port = 54321\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);

    ASSERT_EQ(54321, wfd_config_get_server_port(config));

    wfd_config_dispose(config);
}

TEST(config, tls)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  tls:\n"
        "  {\n"
        "    certificate = \"/path/to/cert.pem\"\n"
        "    key = \"/path/to/key.pem\"\n"
        "  }\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);

    ASSERT_TRUE(wfd_config_is_server_tls_enabled(config));
    ASSERT_STREQ("/path/to/cert.pem", wfd_config_get_server_cert(config));
    ASSERT_STREQ("/path/to/key.pem", wfd_config_get_server_key(config));

    wfd_config_dispose(config);
}

TEST(config, tls_certificate)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  tls:\n"
        "  {\n"
        "    certificate = \"/path/to/cert.pem\"\n"
        "  }\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);

    ASSERT_FALSE(wfd_config_is_server_tls_enabled(config));
    ASSERT_STREQ("/path/to/cert.pem", wfd_config_get_server_cert(config));
    ASSERT_EQ(nullptr, wfd_config_get_server_key(config));

    wfd_config_dispose(config);
}

TEST(config, tls_key)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  tls:\n"
        "  {\n"
        "    key = \"/path/to/key.pem\"\n"
        "  }\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);

    ASSERT_FALSE(wfd_config_is_server_tls_enabled(config));
    ASSERT_EQ(nullptr, wfd_config_get_server_cert(config));
    ASSERT_STREQ("/path/to/key.pem", wfd_config_get_server_key(config));

    wfd_config_dispose(config);
}

TEST(config, document_root)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  document_root = \"/var/www\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);

    ASSERT_STREQ("/var/www", wfd_config_get_server_document_root(config));

    wfd_config_dispose(config);
}
