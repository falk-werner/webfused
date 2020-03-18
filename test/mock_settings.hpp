#ifndef WFD_MOCK_AUTH_SETTINGS_HPP
#define WFD_MOCK_AUTH_SETTINGS_HPP

#include <gmock/gmock.h>
#include "webfused/config/settings.h"

namespace webfused_test
{

class ISettings
{
public:
    virtual ~ISettings() = default;
    virtual char const * get(char const * key) = 0;
};

class MockSettings: public ISettings
{
public:
    MockSettings();
    ~MockSettings() override;
    MOCK_METHOD1(get, char const * (char const * key));
};

}



#endif
