#ifndef WFD_MOCK_LIBCONFIG_HPP
#define WFD_MOCK_LIBCONFIG_HPP

#include <libconfig.h>
#include <gmock/gmock.h>

namespace webfused_test
{

class ILibConfig
{
public:
    virtual ~ILibConfig() = default;
    virtual config_setting_t * config_setting_get_elem(
        config_setting_t const * setting,
        unsigned int i) = 0;
};

class MockLibConfig: public ILibConfig
{
public:
    MockLibConfig();
    ~MockLibConfig() override;
    MOCK_METHOD2(config_setting_get_elem, config_setting_t * (
        config_setting_t const * setting, 
        unsigned int i));

};

}

#endif
