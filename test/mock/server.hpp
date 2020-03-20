#ifndef WFD_MOCK_SERVER_HPP
#define WFD_MOCK_SERVER_HPP

#include <webfuse/adapter/server.h>
#include <gmock/gmock.h>

namespace webfused_test
{

class IServer
{
public:
    virtual ~IServer() = default;
    virtual struct wf_server * wf_server_create(struct wf_server_config * config) = 0;
};

class MockServer: public IServer
{
public:
    MockServer();
    ~MockServer() override;
    MOCK_METHOD1(wf_server_create, struct wf_server * (struct wf_server_config * config));
};


}

#endif
