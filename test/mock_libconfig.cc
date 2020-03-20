#include "mock_libconfig.hpp"
#include "wrap_utils.hpp"

extern "C"
{

static webfused_test::ILibConfig * wfd_MockLibConfig = nullptr;

WFD_WRAP_FUNC2(wfd_MockLibConfig, config_setting_t *, config_setting_get_elem, 
    config_setting_t const *, unsigned int);

}

namespace webfused_test
{

MockLibConfig::MockLibConfig()
{
    wfd_MockLibConfig = this;
}

MockLibConfig::~MockLibConfig()
{
    wfd_MockLibConfig = nullptr;
}

}