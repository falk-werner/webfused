#ifndef WFD_MOCK_AUTH_SETTINGS_HPP
#define WFD_MOCK_AUTH_SETTINGS_HPP

#include <gmock/gmock.h>
#include "webfused/auth/settings.h"

namespace webfused_test
{

class IAuthSettings
{
public:
    virtual ~IAuthSettings() = default;
    virtual char const * getProvider() = 0;
    virtual char const * get(char const * key) = 0;
};

class MockAuthSettings: public IAuthSettings
{
public:
    MockAuthSettings();
    ~MockAuthSettings() override;
    MOCK_METHOD0(getProvider, char const * ());
    MOCK_METHOD1(get, char const * (char const * key));
};

}



#endif
