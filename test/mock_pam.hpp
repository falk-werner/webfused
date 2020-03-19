#ifndef WFD_MOCK_PAM_HPP
#define WFD_MOCK_PAM_HPP

#include <security/pam_appl.h>
#include <gmock/gmock.h>

namespace webfused_test
{

class IPam
{
public:
    virtual ~IPam() = default;
    virtual int start(
        char const * service_name,
        char const * user,
        struct pam_conv const * conversation,
        pam_handle_t * * handle) = 0;
    virtual int end(pam_handle_t * handle, int status) = 0;
    virtual int authenticate(pam_handle_t * handle, int flags) = 0;
    virtual int acct_mgmt(pam_handle_t * handle, int flags) = 0;
    virtual char const * strerror(pam_handle_t * handle, int errnum) = 0;
};

class MockPam: public IPam
{
public:
    MockPam();
    ~MockPam() override;

    MOCK_METHOD4(start, int (
        char const * service_name,
        char const * user,
        struct pam_conv const * conversation,
        pam_handle_t * * handle));
    
    MOCK_METHOD2(end, int(pam_handle_t * handle, int status));
    MOCK_METHOD2(authenticate, int(pam_handle_t * handle, int flags));
    MOCK_METHOD2(acct_mgmt, int (pam_handle_t * handle, int flags));
    MOCK_METHOD2(strerror, char const * (pam_handle_t * handle, int errnum));
};

}

#endif
