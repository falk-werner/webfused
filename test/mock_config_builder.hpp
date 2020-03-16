#ifndef WFD_MOCK_CONFIG_BUILDER_HPP
#define WFD_MOCK_CONFIG_BUILDER_HPP

#include <gmock/gmock.h>
#include "webfused/config/builder.h"

namespace webfused_test
{

class IConfigBuilder
{
public:
    virtual ~IConfigBuilder() = default;
    virtual void setServerVhostname(char const * vhostname) = 0;
    virtual void setServerPort(int port) = 0;
    virtual void setServerKey(char const * key_path) = 0;
    virtual void setServerCert(char const * cert_path) = 0;
    virtual void setServerDocumentRoot(char const * document_root) = 0;
    virtual bool addAuthProvider(wfd_auth_settings * settings) = 0;
};

class MockConfigBuilder: public IConfigBuilder
{
public:
    ~MockConfigBuilder() override = default;
    MOCK_METHOD1(setServerVhostname, void (char const * vhostname));
    MOCK_METHOD1(setServerPort, void (int port));
    MOCK_METHOD1(setServerKey, void (char const * key_path));
    MOCK_METHOD1(setServerCert, void (char const * cert_path));
    MOCK_METHOD1(setServerDocumentRoot, void (char const * document_root));
    MOCK_METHOD1(addAuthProvider, bool (wfd_auth_settings * settings));

    struct wfd_config_builder getBuilder();
};

}

#endif
