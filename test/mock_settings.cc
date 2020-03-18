#include "mock_settings.hpp"

extern "C"
{
using webfused_test::ISettings;

static ISettings * wfd_mock_settings = nullptr;

extern char const *
__real_wfd_settings_get(
    struct wfd_settings * settings,
    char const * key);

char const *
__wrap_wfd_settings_get(
    struct wfd_settings * settings,
    char const * key)
{
    if (nullptr == wfd_mock_settings)
    {
        return __real_wfd_settings_get(settings, key);
    }
    else
    {
        return wfd_mock_settings->get(key);
    }
}


}

namespace webfused_test
{

MockSettings::MockSettings()
{
    wfd_mock_settings = this;
}

MockSettings::~MockSettings()
{
    wfd_mock_settings = nullptr;
}

}