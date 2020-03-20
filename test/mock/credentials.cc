#include "mock/credentials.hpp"
#include "util/wrap.hpp"

extern "C"
{
static webfused_test::ICredentials * wfd_MockCredentials = nullptr;

WFD_WRAP_FUNC1(wfd_MockCredentials, char const *, wf_credentials_type, struct wf_credentials const *);
WFD_WRAP_FUNC2(wfd_MockCredentials, char const *, wf_credentials_get, struct wf_credentials const *, char const *);
}


namespace webfused_test
{

MockCredentials::MockCredentials()
{
    wfd_MockCredentials = this;
}

MockCredentials::~MockCredentials()
{
    wfd_MockCredentials = nullptr;
}

}
