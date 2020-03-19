#include "mock_pam.hpp"

extern "C"
{

static webfused_test::IPam * wfd_MockPam = nullptr;

extern int __real_pam_start(
    char const * service_name,
    char const * user,
    struct pam_conv const * conversation,
    pam_handle_t * * handle);
extern int __real_pam_end(pam_handle_t * handle, int status);
extern int __real_pam_authenticate(pam_handle_t * handle, int flags);
extern int __real_pam_acct_mgmt(pam_handle_t * handle, int flags);
extern char const * __real_pam_strerror(pam_handle_t * handle, int errnum);

int __wrap_pam_start(
    char const * service_name,
    char const * user,
    struct pam_conv const * conversation,
    pam_handle_t * * handle)
{
    if (nullptr == wfd_MockPam)
    {
        return __real_pam_start(service_name, user, conversation, handle);
    }
    else
    {
        return wfd_MockPam->start(service_name, user, conversation, handle);
    }

}

int __wrap_pam_end(pam_handle_t * handle, int status)
{
    if (nullptr == wfd_MockPam)
    {
        return __real_pam_end(handle, status);
    }
    else
    {
        return wfd_MockPam->end(handle, status);
    }
}

int __wrap_pam_authenticate(pam_handle_t * handle, int flags)
{
    if (nullptr == wfd_MockPam)
    {
        return __real_pam_authenticate(handle, flags);
    }
    else
    {
        return wfd_MockPam->authenticate(handle, flags);
    }
}

int __wrap_pam_acct_mgmt(pam_handle_t * handle, int flags)
{
    if (nullptr == wfd_MockPam)
    {
        return __real_pam_acct_mgmt(handle, flags);
    }
    else
    {
        return wfd_MockPam->acct_mgmt(handle, flags);
    }
}

char const * __wrap_pam_strerror(pam_handle_t * handle, int errnum)
{
    if (nullptr == wfd_MockPam)
    {
        return __real_pam_strerror(handle, errnum);
    }
    else
    {
        return wfd_MockPam->strerror(handle, errnum);
    }
}


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
