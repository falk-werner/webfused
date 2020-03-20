#ifndef WFD_MOCK_CONFIG_BUILDER_HPP
#define WFD_MOCK_CONFIG_BUILDER_HPP

#include <gmock/gmock.h>
#include "webfused/config/config_intern.h"

namespace webfused_test
{

class IConfigBuilder
{
public:
    virtual ~IConfigBuilder() = default;
    virtual wfd_config * create(void) = 0;
    virtual void dispose(wfd_config * config) = 0;
    virtual void setServerVhostname(char const * vhostname) = 0;
    virtual void setServerPort(int port) = 0;
    virtual void setServerKey(char const * key_path) = 0;
    virtual void setServerCert(char const * cert_path) = 0;
    virtual void setServerDocumentRoot(char const * document_root) = 0;
    virtual bool addAuthProvider(char const * provider, wfd_settings * settings) = 0;
    virtual bool addFilesystem(char const * name, char const * mountpoint) = 0;
    virtual bool setLogger(char const * provider, int level, wfd_settings * settings) = 0;
    virtual void setUser(char const * user, char const * group) = 0;
};

class MockConfigBuilder: public IConfigBuilder
{
public:
    MockConfigBuilder();
    ~MockConfigBuilder() override;
    MOCK_METHOD0(create, wfd_config * (void));
    MOCK_METHOD1(dispose, void (wfd_config * config));
    MOCK_METHOD1(setServerVhostname, void (char const * vhostname));
    MOCK_METHOD1(setServerPort, void (int port));
    MOCK_METHOD1(setServerKey, void (char const * key_path));
    MOCK_METHOD1(setServerCert, void (char const * cert_path));
    MOCK_METHOD1(setServerDocumentRoot, void (char const * document_root));
    MOCK_METHOD2(addAuthProvider, bool (char const * provider, wfd_settings * settings));
    MOCK_METHOD2(addFilesystem, bool (char const * name, char const * mountpoint));
    MOCK_METHOD3(setLogger, bool (char const * provider, int level, wfd_settings * settings));
    MOCK_METHOD2(setUser, void (char const * user, char const * group));

    struct wfd_config * getBuilder();
};

}

#endif
