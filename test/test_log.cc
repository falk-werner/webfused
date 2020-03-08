#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "webfused/log/logger.h"
#include "webfused/log/log.h"

using ::testing::_;
using ::testing::StrEq;

namespace
{

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void log(int level, char const * format, va_list args) = 0;
    virtual void onclose() = 0;
};

class MockLogger: public ILogger
{
public:
    MOCK_METHOD3(log, void(int level, char const * format, va_list args));
    MOCK_METHOD0(onclose, void(void));

    void * getUserData() 
    {
        ILogger * logger = this;
        return reinterpret_cast<void*>(logger);
    }
};

}

extern "C"
{

static void MockLogger_log(
    void * user_data,
    int level,
    char const * format,
    va_list args)
{
    ILogger * logger = reinterpret_cast<ILogger*>(user_data);
    logger->log(level, format, args);
}

static void MockLogger_onclose(
    void * user_data)
{
    ILogger * logger = reinterpret_cast<ILogger*>(user_data);
    logger->onclose();
}

}

TEST(log, fatal)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_FATAL, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &MockLogger_log, &MockLogger_onclose, logger.getUserData());
    WFD_FATAL("too bad");
    wfd_logger_close();
}

TEST(log, error)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &MockLogger_log, &MockLogger_onclose, logger.getUserData());
    WFD_ERROR("too bad");
    wfd_logger_close();
}

TEST(log, warn)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &MockLogger_log, &MockLogger_onclose, logger.getUserData());
    WFD_WARN("too bad");
    wfd_logger_close();
}

TEST(log, info)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_INFO, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &MockLogger_log, &MockLogger_onclose, logger.getUserData());
    WFD_INFO("too bad");
    wfd_logger_close();
}

TEST(log, debug)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_DEBUG, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_ALL, &MockLogger_log, &MockLogger_onclose, logger.getUserData());
    WFD_DEBUG("too bad");
    wfd_logger_close();
}

TEST(log, respect_loglevel)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(_, _, _)).Times(0);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_WARN, &MockLogger_log, &MockLogger_onclose, logger.getUserData());
    WFD_DEBUG("too bad");
    wfd_logger_close();
}

TEST(log, log_same_loglevel)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_logger_init(WFD_LOGLEVEL_WARN, &MockLogger_log, &MockLogger_onclose, logger.getUserData());
    WFD_WARN("too bad");
    wfd_logger_close();
}

TEST(log, omit_onclose_if_nullptr)
{
    MockLogger logger;

    EXPECT_CALL(logger, log(WFD_LOGLEVEL_WARN, StrEq("too bad"), _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(0);

    wfd_logger_init(WFD_LOGLEVEL_WARN, &MockLogger_log, nullptr, logger.getUserData());
    WFD_WARN("too bad");
    wfd_logger_close();
}
