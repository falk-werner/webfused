#include <gtest/gtest.h>

#include "webfused/config/factory.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "mock_logger.hpp"
#include "mock_config_builder.hpp"


using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::StrEq;
using ::webfused_test::MockLogger;
using ::webfused_test::MockConfigBuilder;

TEST(config, is_loadable)
{
    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setLogger(_,_,_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(builder, setServerVhostname(StrEq("localhost"))).Times(1);
    EXPECT_CALL(builder, setServerPort(8080)).Times(1);
    EXPECT_CALL(builder, addAuthProvider(_, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(builder, addFilesystem(_,_)).Times(1).WillOnce(Return(true));

    bool result = wfd_config_load_file(builder.getBuilder(), "webfused.conf");
    ASSERT_TRUE(result);
}

TEST(config, minimal_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;

    char const minimal[] = "version = { major = 1, minor = 0 }\n";
    bool result = wfd_config_load_string(builder.getBuilder(), minimal);
    ASSERT_TRUE(result);
}

TEST(config, invalid_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    

    char const syntax_error[] = "version.major = 1\n";

    bool result = wfd_config_load_string(builder.getBuilder(), syntax_error);
    ASSERT_FALSE(result);    
}

TEST(config, invalid_config_file)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    

    bool result = wfd_config_load_file(builder.getBuilder(), "invalid.conf");
    ASSERT_FALSE(result);    
}

TEST(config, invalid_major_version_too_low)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    

    char const too_low[] = "version = { major = 0, minor = 0 }\n";

    bool result = wfd_config_load_string(builder.getBuilder(), too_low);
    ASSERT_FALSE(result);
}

TEST(config, invalid_major_version_too_high)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    

    char const too_high[] = "version = { major = 2, minor = 0 }\n";

    bool result = wfd_config_load_string(builder.getBuilder(), too_high);
    ASSERT_FALSE(result);
}

TEST(config, invalid_missing_major_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    

    char const too_high[] = "version = { minor = 0 }\n";

    bool result = wfd_config_load_string(builder.getBuilder(), too_high);
    ASSERT_FALSE(result);
}

TEST(config, invalid_missing_minor_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    

    char const too_high[] = "version = { major = 1 }\n";

    bool result = wfd_config_load_string(builder.getBuilder(), too_high);
    ASSERT_FALSE(result);
}

TEST(config, valid_older_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_INFO, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);
    
    StrictMock<MockConfigBuilder> builder;    

    char const valid[] = "version = { major = 1, minor = -1 }\n";

    bool result = wfd_config_load_string(builder.getBuilder(), valid);
    ASSERT_TRUE(result);
}

TEST(config, valid_newer_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    

    char const valid[] = "version = { major = 1, minor = 1 }\n";

    bool result = wfd_config_load_string(builder.getBuilder(), valid);
    ASSERT_TRUE(result);
}

TEST(config, vhost_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setServerVhostname(StrEq("some.host"))).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  vhost_name = \"some.host\"\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, port)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setServerPort(54321)).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  port = 54321\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, tls_certificate)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    
    EXPECT_CALL(builder, setServerCert(StrEq("/path/to/cert.pem"))).Times(1);

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
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, tls_key)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    
    EXPECT_CALL(builder, setServerKey(StrEq("/path/to/key.pem"))).Times(1);

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
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, document_root)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setServerDocumentRoot(StrEq("/var/www"))).Times(1);    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  document_root = \"/var/www\"\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, authentication)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, addAuthProvider(_, _)).Times(1).WillOnce(Return(true));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, failed_create_authenticator)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, addAuthProvider(_, _)).Times(1).WillOnce(Return(false));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, failed_missing_auth_provider)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, failed_missing_auth_settings)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "  }\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, filesystems)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(1).WillOnce(Return(true));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, filesystems_empty)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, filesystems_failed_add)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(1).WillOnce(Return(false));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, filesystems_failed_missing_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, filesystems_failed_missing_mountpoint)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\"}\n"
        ")\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, log)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(1).WillOnce(Return(true));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "  level    = \"all\"\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_TRUE(result);
}

TEST(config, log_fail_set_logger)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(1).WillOnce(Return(false));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "  level    = \"all\"\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, log_fail_missing_provider)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  level    = \"all\"\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, log_fail_missing_level)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "  level    = \"fancy\"\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

TEST(config, log_fail_invalid_level)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "}\n"
        ;
    bool result = wfd_config_load_string(builder.getBuilder(), config_text);
    ASSERT_FALSE(result);
}

