#include "mock/server.hpp"
#include "util/wrap.hpp"

extern "C"
{
static webfused_test::IServer * wfd_MockServer = nullptr;

WFD_WRAP_FUNC1(wfd_MockServer, struct wf_server *, wf_server_create, struct wf_server_config *);

}

namespace webfused_test
{

MockServer::MockServer()
{
    wfd_MockServer = this;
}

MockServer::~MockServer()
{
    wfd_MockServer = nullptr;
}

}