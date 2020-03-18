#include "mock_settings.hpp"

extern "C"
{
using webfused_test::ISettings;

static ISettings * wfd_mock_settings = nullptr;

extern char const *
__real_wfd_settings_get_string(
    struct wfd_settings * settings,
    char const * key);

extern char const *
__real_wfd_settings_get_string_or_default(
    struct wfd_settings * settings,
    char const * key,
    char const * default_value);

extern bool
__real_wfd_settings_get_bool(
    struct wfd_settings * settings,
    char const * key);

char const *
__wrap_wfd_settings_get_string(
    struct wfd_settings * settings,
    char const * key)
{
    if (nullptr == wfd_mock_settings)
    {
        return __real_wfd_settings_get_string(settings, key);
    }
    else
    {
        return wfd_mock_settings->getString(key);
    }
}

char const *
__wrap_wfd_settings_get_string_or_default(
    struct wfd_settings * settings,
    char const * key,
    char const * default_value)
{
    if (nullptr == wfd_mock_settings)
    {
        return __real_wfd_settings_get_string_or_default(
            settings, key, default_value);
    }
    else
    {
        return wfd_mock_settings->getStringOrDefault(key, default_value);
    }
}

bool
__wrap_wfd_settings_get_bool(
    struct wfd_settings * settings,
    char const * key)
{
    if (nullptr == wfd_mock_settings)
    {
        return __real_wfd_settings_get_bool(settings, key);
    }
    else
    {
        return wfd_mock_settings->getBool(key);
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