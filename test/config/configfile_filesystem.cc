#include <gtest/gtest.h>

#include "webfused/config/config.h"
#include "webfused/config/factory.h"
#include "webfused/util/string_list.h"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"
#include "mock/logger.hpp"
#include "mock/config_builder.hpp"
#include "mock/libconfig.hpp"

using ::testing::Invoke;
using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::StrEq;
using ::webfused_test::MockLogger;
using ::webfused_test::MockConfigBuilder;
using ::webfused_test::MockLibConfig;

TEST(configfile_fs, filesystems)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(1).WillOnce(Return(true));

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_fs, filesystems_empty)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(0);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}

TEST(configfile_fs, filesystems_failed_add)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(1).WillOnce(Return(false));

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\", mount_options = () }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_fs, filesystems_failed_missing_name)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(0);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        "  {mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_fs, filesystems_failed_missing_mountpoint)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(0);

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\"}\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_fs, filesystems_failed_missing_elem)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(0);

    MockLibConfig libconfig;
    EXPECT_CALL(libconfig, config_setting_get_elem(_,_)).Times(1).WillOnce(Return(nullptr));

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\" }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_fs, filesystems_failed_add_options)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_dispose(_)).Times(1);
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(1).WillOnce(Return(false));

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\", mount_options = (\"-o\", \"allow_other\") }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_EQ(nullptr, config);
}

TEST(configfile_fs, with_mountoptions)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    StrictMock<MockConfigBuilder> builder;
    EXPECT_CALL(builder, wfd_config_create).Times(1).WillOnce(Return(builder.getBuilder()));
    EXPECT_CALL(builder, wfd_config_add_filesystem(_,_,_,_)).Times(1).WillOnce(Invoke(
        [](wfd_config * config, char const * name, char const * mountpoint, wfd_string_list const * mount_options) -> bool {
            std::cout << mount_options->items[0] << std::endl;
            std::cout << mount_options->items[1] << std::endl;
            return (2 == mount_options->size);
    }));

    char const config_text[] =
        "version = { major = " WFD_CONFIG_VERSION_STR_MAJOR ", minor = " WFD_CONFIG_VERSION_STR_MINOR " }\n"
        "filesystems:\n"
        "(\n"
        "  {name = \"foo\", mount_point = \"/tmp/test\", mount_options = (\"-o\", \"allow_user\") }\n"
        ")\n"
        ;
    struct wfd_config * config = wfd_config_load_string(config_text);
    ASSERT_NE(nullptr, config);
}
