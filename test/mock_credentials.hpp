#ifndef WFD_MOCK_CREDENTIALS_HPP
#define WFD_MOCK_CREDENTIALS_HPP

#include "gmock/gmock.h"
#include "webfuse/adapter/credentials.h"

namespace webfused_test
{

class ICredentials
{
public:
    virtual ~ICredentials() = default;
    virtual char const * type() = 0;
    virtual char const * get(char const * key) = 0;   
};

class MockCredentials: public ICredentials
{
public:
    MockCredentials();
    virtual ~MockCredentials();
    MOCK_METHOD0(type, char const*());
    MOCK_METHOD1(get, char const *(char const * key));
};

}

#endif
