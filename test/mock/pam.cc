#include "mock/pam.hpp"
#include "util/wrap.hpp"

extern "C"
{
static webfused_test::IPam * wfd_MockPam = nullptr;

WFD_WRAP_FUNC4(wfd_MockPam, int, pam_start, char const *, char const *, struct pam_conv const *, pam_handle_t **);
WFD_WRAP_FUNC2(wfd_MockPam, int, pam_end, pam_handle_t *, int);
WFD_WRAP_FUNC2(wfd_MockPam, int, pam_authenticate, pam_handle_t *, int);
WFD_WRAP_FUNC2(wfd_MockPam, int, pam_acct_mgmt, pam_handle_t *, int);
WFD_WRAP_FUNC2(wfd_MockPam, char const *, pam_strerror, pam_handle_t *, int);

}

namespace webfused_test
{

MockPam::MockPam()
{
    wfd_MockPam = this;
}

MockPam::~MockPam()
{
    wfd_MockPam = nullptr;
}

}
