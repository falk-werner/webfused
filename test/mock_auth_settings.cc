#include "mock_auth_settings.hpp"

extern "C"
{
using webfused_test::IAuthSettings;

static IAuthSettings * wfd_mock_auth_settings = nullptr;

extern char const *
__real_wfd_auth_settings_get_provider(
    struct wfd_auth_settings * settings);

extern char const *
__real_wfd_auth_settings_get(
    struct wfd_auth_settings * settings,
    char const * key);

char const *
__wrap_wfd_auth_settings_get_provider(
    struct wfd_auth_settings * settings)
{
    if (nullptr == wfd_mock_auth_settings)
    {
        return __real_wfd_auth_settings_get_provider(settings);
    }
    else
    {
        return wfd_mock_auth_settings->getProvider();
    }
}

char const *
__wrap_wfd_auth_settings_get(
    struct wfd_auth_settings * settings,
    char const * key)
{
    if (nullptr == wfd_mock_auth_settings)
    {
        return __real_wfd_auth_settings_get(settings, key);
    }
    else
    {
        return wfd_mock_auth_settings->get(key);
    }
}


}

namespace webfused_test
{

MockAuthSettings::MockAuthSettings()
{
    wfd_mock_auth_settings = this;
}

MockAuthSettings::~MockAuthSettings()
{
    wfd_mock_auth_settings = nullptr;
}

}