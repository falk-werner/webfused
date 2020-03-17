#include "mock_config_builder.hpp"

extern "C"
{
using webfused_test::IConfigBuilder;

static void
wfd_MockConfigBuilder_set_server_vhostname(
    void * data,
    char const * vhost_name)
{
    auto * builder = reinterpret_cast<IConfigBuilder*>(data);
    builder->setServerVhostname(vhost_name);
}

static void
wfd_MockConfigBuilder_set_server_port(
    void * data,
    int port)
{
    auto * builder = reinterpret_cast<IConfigBuilder*>(data);
    builder->setServerPort(port);
}

static void
wfd_MockConfigBuilder_set_server_key(
    void * data,
    char const * key_path)
{
    auto * builder = reinterpret_cast<IConfigBuilder*>(data);
    builder->setServerKey(key_path);
}

static void
wfd_MockConfigBuilder_set_server_cert(
    void * data,
    char const * cert_path)
{
    auto * builder = reinterpret_cast<IConfigBuilder*>(data);
    builder->setServerCert(cert_path);
}

static void
wfd_MockConfigBuilder_set_server_document_root(
    void * data,
    char const * document_root)
{
    auto * builder = reinterpret_cast<IConfigBuilder*>(data);
    builder->setServerDocumentRoot(document_root);
}

static bool
wfd_MockConfigBuilder_add_auth_provider(
    void * data,
    struct wfd_auth_settings * settings)
{
    auto * builder = reinterpret_cast<IConfigBuilder*>(data);
    return builder->addAuthProvider(settings);
}

static bool
wfd_MockConfigBuilder_add_filesystem(
    void * data,
    char const * name,
    char const * mountpoint)
{
    auto * builder = reinterpret_cast<IConfigBuilder*>(data);
    return builder->addFilesystem(name, mountpoint);
}


static const wfd_config_builder_vtable wfd_MockConfigBuilder_vtable =
{
    &wfd_MockConfigBuilder_set_server_vhostname,
    &wfd_MockConfigBuilder_set_server_port,
    &wfd_MockConfigBuilder_set_server_key,
    &wfd_MockConfigBuilder_set_server_cert,
    &wfd_MockConfigBuilder_set_server_document_root,
    &wfd_MockConfigBuilder_add_auth_provider,
    &wfd_MockConfigBuilder_add_filesystem
};

}

namespace webfused_test
{
    struct wfd_config_builder MockConfigBuilder::getBuilder()
    {
        IConfigBuilder * config_builder = this;

        wfd_config_builder builder = {
            &wfd_MockConfigBuilder_vtable,
            reinterpret_cast<void*>(config_builder)
        };

        return builder;
    }
}