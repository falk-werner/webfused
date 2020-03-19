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
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, setLogger(_,_,_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(builder, setServerVhostname(StrEq("localhost"))).Times(1);
    EXPECT_CALL(builder, setServerPort(8080)).Times(1);
    EXPECT_CALL(builder, addAuthProvider(_, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(builder, addFilesystem(_,_)).Times(1).WillOnce(Return(true));

    struct wfd_config * config = wfd_config_load_file("webfused.conf");
    ASSERT_NE(nullptr, config);
}

TEST(config, minimal_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));

    char const minimal[] = "version = { major = 1, minor = 0 }\n";
    struct wfd_config * config = wfd_config_load_string(minimal);
    ASSERT_NE(nullptr, config);
}

TEST(config, invalid_config)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(0);
    EXPECT_CALL(builder, dispose(_)).Times(0);

    char const syntax_error[] = "version.major = 1\n";

    wfd_config * config = wfd_config_load_string(syntax_error);
    ASSERT_EQ(nullptr, config);    
}

TEST(config, invalid_config_file)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(0);
    EXPECT_CALL(builder, dispose(_)).Times(0);

    struct wfd_config * config = wfd_config_load_file("invalid.conf");
    ASSERT_EQ(nullptr, config);
}

TEST(config, invalid_major_version_too_low)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);

    char const too_low[] = "version = { major = 0, minor = 0 }\n";

    struct wfd_config * config= wfd_config_load_string(too_low);
    ASSERT_EQ(nullptr, config);
}

TEST(config, invalid_major_version_too_high)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);

    char const too_high[] = "version = { major = 2, minor = 0 }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);
}

TEST(config, invalid_missing_major_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);

    char const too_high[] = "version = { minor = 0 }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);
}

TEST(config, invalid_missing_minor_version)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);

    char const too_high[] = "version = { major = 1 }\n";

    struct wfd_config * config = wfd_config_load_string(too_high);
    ASSERT_EQ(nullptr, config);
}

TEST(config, valid_older_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_INFO, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);
    
    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));

    char const valid[] = "version = { major = 1, minor = -1 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);
}

TEST(config, valid_newer_minor)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));

    char const valid[] = "version = { major = 1, minor = 1 }\n";

    struct wfd_config * config = wfd_config_load_string(valid);
    ASSERT_NE(nullptr, config);
}

TEST(config, vhost_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, setServerVhostname(StrEq("some.host"))).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  vhost_name = \"some.host\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, port)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, setServerPort(54321)).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  port = 54321\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, tls_certificate)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
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
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, tls_key)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;    
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
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
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, document_root)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, setServerDocumentRoot(StrEq("/var/www"))).Times(1);    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "server:\n"
        "{\n"
        "  document_root = \"/var/www\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, authentication)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
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
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, failed_create_authenticator)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
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
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, failed_missing_auth_provider)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    settings: { }\n"
        "  }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, failed_missing_auth_settings)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "authentication:\n"
        "(\n"
        "  {\n"
        "    provider = \"test\"\n"
        "  }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, filesystems)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(1).WillOnce(Return(true));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, filesystems_empty)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, filesystems_failed_add)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(1).WillOnce(Return(false));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, filesystems_failed_missing_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, filesystems_failed_missing_mountpoint)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, addFilesystem(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\"}\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, log)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(1).WillOnce(Return(true));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "  level    = \"all\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, log_fail_set_logger)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(1).WillOnce(Return(false));    

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "  level    = \"all\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, log_fail_missing_provider)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  level    = \"all\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, log_fail_missing_level)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "  level    = \"fancy\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, log_fail_invalid_level)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, setLogger(_, _, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "log:\n"
        "{\n"
        "  provider = \"stderr\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, set_user)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, setUser(_, _)).Times(1);   

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "user:\n"
        "{\n"
        "  name  = \"webfused\"\n"
        "  group = \"webfused\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(config, set_user_fail_missing_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, setUser(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "user:\n"
        "{\n"
        "  group = \"webfused\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(config, set_user_fail_missing_group)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, dispose(_)).Times(1);
    EXPECT_CALL(builder, setUser(_, _)).Times(0);

    char const config_text[] = 
        "version = { major = 1, minor = 0 }\n"
        "user:\n"
        "{\n"
        "  name = \"webfused\"\n"
        "}\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}
