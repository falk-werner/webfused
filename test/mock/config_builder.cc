#include "mock/config_builder.hpp"
#include "util/wrap.hpp"

extern "C"
{
static webfused_test::IConfigBuilder * wfd_MockConfigBuilder = nullptr;

WFD_WRAP_FUNC0(wfd_MockConfigBuilder, wfd_config *, wfd_config_create);
WFD_WRAP_FUNC1(wfd_MockConfigBuilder, void, wfd_config_dispose, wfd_config *);
WFD_WRAP_FUNC2(wfd_MockConfigBuilder, void, wfd_config_set_server_vhostname, wfd_config *, char const *);
WFD_WRAP_FUNC2(wfd_MockConfigBuilder, void, wfd_config_set_server_port, wfd_config *, int);
WFD_WRAP_FUNC2(wfd_MockConfigBuilder, void, wfd_config_set_server_key, wfd_config *, char const *);
WFD_WRAP_FUNC2(wfd_MockConfigBuilder, void, wfd_config_set_server_cert, wfd_config *, char const *);
WFD_WRAP_FUNC2(wfd_MockConfigBuilder, void, wfd_config_set_server_document_root, wfd_config *, char const *);
WFD_WRAP_FUNC3(wfd_MockConfigBuilder, bool, wfd_config_add_auth_provider, wfd_config *, char const *, wfd_settings *);
WFD_WRAP_FUNC3(wfd_MockConfigBuilder, bool, wfd_config_add_filesystem, wfd_config *, char const *, char const *);
WFD_WRAP_FUNC4(wfd_MockConfigBuilder, bool, wfd_config_set_logger, wfd_config *, char const *, int, wfd_settings *);
WFD_WRAP_FUNC3(wfd_MockConfigBuilder, void, wfd_config_set_user, wfd_config *, char const *, char const *);
}

namespace webfused_test
{

MockConfigBuilder::MockConfigBuilder()
{
    wfd_MockConfigBuilder = this;    
}

MockConfigBuilder::~MockConfigBuilder()
{
    wfd_MockConfigBuilder = nullptr;
}

struct wfd_config * MockConfigBuilder::getBuilder()
{
    IConfigBuilder * config_builder = this;

    return reinterpret_cast<wfd_config *>(config_builder);
}
}