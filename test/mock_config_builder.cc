#include "mock_config_builder.hpp"

extern "C"
{
using webfused_test::IConfigBuilder;

static IConfigBuilder * wfd_MockConfigBuilder = nullptr;

extern struct wfd_config *
__real_wfd_config_create(void);

extern void
__real_wfd_config_dispose(
    struct wfd_config * config);

extern void
__real_wfd_config_set_server_vhostname(
    struct wfd_config * config,
    char const * vhost_name);

extern void
__real_wfd_config_set_server_port(
    struct wfd_config * config,
    int port);

extern void
__real_wfd_config_set_server_key(
    struct wfd_config * config,
    char const * key_path);

extern void
__real_wfd_config_set_server_cert(
    struct wfd_config * config,
    char const * cert_path);

extern void
__real_wfd_config_set_server_document_root(
    struct wfd_config * config,
    char const * document_root);

extern bool
__real_wfd_config_add_auth_provider(
    struct wfd_config * config,
    char const * provider,
    struct wfd_settings * settings);

extern bool
__real_wfd_config_add_filesystem(
    struct wfd_config * config,
    char const * name,
    char const * mount_point);

extern bool
__real_wfd_config_set_logger(
    struct wfd_config * config,
    char const * provider,
    int level,
    struct wfd_settings * settings);

extern void
__real_wfd_config_set_user(
    struct wfd_config * config,
    char const * user,
    char const * group);

struct wfd_config *
__wrap_wfd_config_create(void)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        return __real_wfd_config_create();
    }
    else
    {
        return wfd_MockConfigBuilder->create();
    }
}

void
__wrap_wfd_config_dispose(
    struct wfd_config * config)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        __real_wfd_config_dispose(config);
    }
    else
    {
        wfd_MockConfigBuilder->dispose(config);
    }
}

void
__wrap_wfd_config_set_server_vhostname(
    struct wfd_config * config,
    char const * vhost_name)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        __real_wfd_config_set_server_vhostname(config, vhost_name);
    }
    else
    {
        wfd_MockConfigBuilder->setServerVhostname(vhost_name);
    }
}

void
__wrap_wfd_config_set_server_port(
    struct wfd_config * config,
    int port)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        __real_wfd_config_set_server_port(config, port);
    }
    else
    {
        wfd_MockConfigBuilder->setServerPort(port);
    }
}

void
__wrap_wfd_config_set_server_key(
    struct wfd_config * config,
    char const * key_path)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        __real_wfd_config_set_server_key(config, key_path);
    }
    else
    {
        wfd_MockConfigBuilder->setServerKey(key_path);
    }
}

void
__wrap_wfd_config_set_server_cert(
    struct wfd_config * config,
    char const * cert_path)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        __real_wfd_config_set_server_cert(config, cert_path);
    }
    else
    {
        wfd_MockConfigBuilder->setServerCert(cert_path);
    }
}

void
__wrap_wfd_config_set_server_document_root(
    struct wfd_config * config,
    char const * document_root)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        __real_wfd_config_set_server_document_root(config, document_root);
    }
    else
    {
        wfd_MockConfigBuilder->setServerDocumentRoot(document_root);
    }
}

bool
__wrap_wfd_config_add_auth_provider(
    struct wfd_config * config,
    char const * provider,
    struct wfd_settings * settings)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        return __real_wfd_config_add_auth_provider(config, provider, settings);
    }
    else
    {
        return wfd_MockConfigBuilder->addAuthProvider(provider, settings);
    }
}

bool
__wrap_wfd_config_add_filesystem(
    struct wfd_config * config,
    char const * name,
    char const * mount_point)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        return __real_wfd_config_add_filesystem(config, name, mount_point);
    }
    else
    {
        return wfd_MockConfigBuilder->addFilesystem(name, mount_point);
    }
}

bool
__wrap_wfd_config_set_logger(
    struct wfd_config * config,
    char const * provider,
    int level,
    struct wfd_settings * settings)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        return __real_wfd_config_set_logger(config, provider, level, settings);
    }
    else
    {
        return wfd_MockConfigBuilder->setLogger(provider, level, settings);
    }
}

void
__wrap_wfd_config_set_user(
    struct wfd_config * config,
    char const * user,
    char const * group)
{
    if (nullptr == wfd_MockConfigBuilder)
    {
        __real_wfd_config_set_user(config, user, group);
    }
    else
    {
        wfd_MockConfigBuilder->setUser(user, group);
    }
}

}

namespace webfused_test
{

MockConfigBuilder::MockConfigBuilder()
{
    wfd_MockConfigBuilder = this;    
}

MockConfigBuilder::~MockConfigBuilder()
{
    wfd_MockConfigBuilder = nullptr;
}

struct wfd_config * MockConfigBuilder::getBuilder()
{
    IConfigBuilder * config_builder = this;

    return reinterpret_cast<wfd_config *>(config_builder);
}
}