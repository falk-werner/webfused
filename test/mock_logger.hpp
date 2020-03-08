#ifndef WFD_MOCK_LOGGER_HPP
#define WFD_MOCK_LOGGER_HPP

#include <gmock/gmock.h>
#include <cstdarg>

namespace webfused_test
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
    explicit MockLogger(bool omit_init = false);
    ~MockLogger() override;
    MOCK_METHOD3(log, void(int level, char const * format, va_list args));
    MOCK_METHOD0(onclose, void(void));

    void * getUserData();
private:
    bool close_on_destruct;
};

}

extern "C"
{

extern void wfd_MockLogger_log(
    void * user_data,
    int level,
    char const * format,
    va_list args);

extern void wfd_MockLogger_onclose(
    void * user_data);

}

#endif
