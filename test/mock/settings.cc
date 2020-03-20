#include "mock/settings.hpp"
#include "util/wrap.hpp"

extern "C"
{
static webfused_test::ISettings * wfd_MockSettings = nullptr;

WFD_WRAP_FUNC2(wfd_MockSettings, char const *, wfd_settings_get_string, struct wfd_settings *, char const *);
WFD_WRAP_FUNC3(wfd_MockSettings, char const *, wfd_settings_get_string_or_default, 
    struct wfd_settings *, char const *, char const *);
WFD_WRAP_FUNC2(wfd_MockSettings, bool, wfd_settings_get_bool, struct wfd_settings *, char const *);
}

namespace webfused_test
{

MockSettings::MockSettings()
{
    wfd_MockSettings = this;
}

MockSettings::~MockSettings()
{
    wfd_MockSettings = nullptr;
}

}