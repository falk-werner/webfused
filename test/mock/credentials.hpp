#ifndef WFD_MOCK_CREDENTIALS_HPP
#define WFD_MOCK_CREDENTIALS_HPP

#include <gmock/gmock.h>
#include "webfuse/credentials.h"

namespace webfused_test
{

class ICredentials
{
public:
    virtual ~ICredentials() = default;
    virtual char const * wf_credentials_type(struct wf_credentials const * credentials) = 0;
    virtual char const * wf_credentials_get(struct wf_credentials const * credentials, char const * key) = 0;   
};

class MockCredentials: public ICredentials
{
public:
    MockCredentials();
    virtual ~MockCredentials();
    MOCK_METHOD1(wf_credentials_type, char const*(struct wf_credentials const * credentials));
    MOCK_METHOD2(wf_credentials_get, char const *(struct wf_credentials const * credentials, char const * key));
};

}

#endif
