#include "mock_credentials.hpp"


extern "C"
{
using webfused_test::ICredentials;

static ICredentials * wfd_mock_credentials = nullptr;

extern char const * 
__real_wf_credentials_type(
    struct wf_credentials const * credentials);

char const *
__wrap_wf_credentials_type(
    struct wf_credentials const * credentials)
{
    if (nullptr == wfd_mock_credentials)
    {
        return __real_wf_credentials_type(credentials);
    }
    else
    {
        return wfd_mock_credentials->type();
    }
}

extern char const * __real_wf_credentials_get(
    struct wf_credentials const * credentials,
    char const * key);


char const * __wrap_wf_credentials_get(
    struct wf_credentials const * credentials,
    char const * key)
{
    if (nullptr == wfd_mock_credentials)
    {
        return __real_wf_credentials_get(credentials, key);
    }
    else
    {
        return wfd_mock_credentials->get(key);
    }
}


}


namespace webfused_test
{

MockCredentials::MockCredentials()
{
    wfd_mock_credentials = this;
}

MockCredentials::~MockCredentials()
{
    wfd_mock_credentials = nullptr;
}

}
