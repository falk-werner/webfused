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
    virtual wfd_config * wfd_config_create(void) = 0;
    virtual void wfd_config_dispose(wfd_config * config) = 0;
    virtual void wfd_config_set_server_vhostname(wfd_config * config, char const * vhostname) = 0;
    virtual void wfd_config_set_server_port(wfd_config * config, int port) = 0;
    virtual void wfd_config_set_server_key(wfd_config * config, char const * key_path) = 0;
    virtual void wfd_config_set_server_cert(wfd_config * config, char const * cert_path) = 0;
    virtual void wfd_config_set_server_document_root(wfd_config * config, char const * document_root) = 0;
    virtual bool wfd_config_add_auth_provider(wfd_config * config, char const * provider, wfd_settings * settings) = 0;
    virtual bool wfd_config_add_filesystem(wfd_config * config, char const * name, char const * mountpoint) = 0;
    virtual bool wfd_config_set_logger(wfd_config * config, char const * provider, int level, wfd_settings * settings) = 0;
    virtual void wfd_config_set_user(wfd_config * config, char const * user, char const * group) = 0;
};

class MockConfigBuilder: public IConfigBuilder
{
public:
    MockConfigBuilder();
    ~MockConfigBuilder() override;
    MOCK_METHOD0(wfd_config_create, wfd_config * (void));
    MOCK_METHOD1(wfd_config_dispose, void (wfd_config * config));
    MOCK_METHOD2(wfd_config_set_server_vhostname, void (wfd_config * config, char const * vhostname));
    MOCK_METHOD2(wfd_config_set_server_port, void (wfd_config * config, int port));
    MOCK_METHOD2(wfd_config_set_server_key, void (wfd_config * config, char const * key_path));
    MOCK_METHOD2(wfd_config_set_server_cert, void (wfd_config * config, char const * cert_path));
    MOCK_METHOD2(wfd_config_set_server_document_root, void (wfd_config * config, char const * document_root));
    MOCK_METHOD3(wfd_config_add_auth_provider, bool (wfd_config * config, char const * provider, wfd_settings * settings));
    MOCK_METHOD3(wfd_config_add_filesystem, bool (wfd_config * config, char const * name, char const * mountpoint));
    MOCK_METHOD4(wfd_config_set_logger, bool (wfd_config * config, char const * provider, int level, wfd_settings * settings));
    MOCK_METHOD3(wfd_config_set_user, void (wfd_config * config, char const * user, char const * group));

    struct wfd_config * getBuilder();
};

}

#endif
