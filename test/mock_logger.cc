#include "mock_logger.hpp"
#include "webfused/log/logger.h"
#include "webfused/log/log.h"

namespace webfused_test
{

MockLogger::MockLogger(bool omit_init)
: close_on_destruct(!omit_init)
{
    if (!omit_init)
    {
        wfd_logger_init(
            WFD_LOGLEVEL_ALL,
            &wfd_MockLogger_log,
            &wfd_MockLogger_onclose,
            getUserData());
    }
}

MockLogger::~MockLogger()
{
    if (close_on_destruct)
    {
        wfd_logger_close();
    }
}

void * MockLogger::getUserData() 
{
    ILogger * logger = this;
    return reinterpret_cast<void*>(logger);
}

}

extern "C"
{
using webfused_test::ILogger;

void wfd_MockLogger_log(
    void * user_data,
    int level,
    char const * format,
    va_list args)
{
    ILogger * logger = reinterpret_cast<ILogger*>(user_data);
    logger->log(level, format, args);
}

void wfd_MockLogger_onclose(
    void * user_data)
{
    ILogger * logger = reinterpret_cast<ILogger*>(user_data);
    logger->onclose();
}

}