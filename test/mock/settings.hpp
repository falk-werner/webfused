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
    virtual char const * getString(char const * key) = 0;
    virtual char const * getStringOrDefault(char const * key, char const * default_value) = 0;
    virtual bool getBool(char const * key) = 0;
};

class MockSettings: public ISettings
{
public:
    MockSettings();
    ~MockSettings() override;
    MOCK_METHOD1(getString, char const * (char const * key));
    MOCK_METHOD2(getStringOrDefault, char const * (char const * key, char const * default_value));
    MOCK_METHOD1(getBool, bool (char const * key));
};

}



#endif
