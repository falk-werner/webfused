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
    virtual char const * wfd_settings_get_string(wfd_settings * settings, char const * key) = 0;
    virtual char const * wfd_settings_get_string_or_default(wfd_settings * settings, char const * key, char const * default_value) = 0;
    virtual bool wfd_settings_get_bool(wfd_settings * settings, char const * key) = 0;
};

class MockSettings: public ISettings
{
public:
    MockSettings();
    ~MockSettings() override;
    MOCK_METHOD2(wfd_settings_get_string, char const * (wfd_settings * settings, char const * key));
    MOCK_METHOD3(wfd_settings_get_string_or_default, char const * (wfd_settings * settings, char const * key, char const * default_value));
    MOCK_METHOD2(wfd_settings_get_bool, bool (wfd_settings * settings, char const * key));
};

}



#endif
