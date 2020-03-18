#include "webfused/config/builder.h"

void
wfd_config_builder_set_server_vhostname(
    struct wfd_config_builder builder,
    char const * vhost_name)
{
    builder.vtable->set_server_vhostname(builder.data, vhost_name);
}

void
wfd_config_builder_set_server_port(
    struct wfd_config_builder builder,
    int port)
{
    builder.vtable->set_server_port(builder.data, port);
}

void
wfd_config_builder_set_server_key(
    struct wfd_config_builder builder,
    char const * key_path)
{
    builder.vtable->set_server_key(builder.data, key_path);
}

void
wfd_config_builder_set_server_cert(
    struct wfd_config_builder builder,
    char const * cert_path)
{
    builder.vtable->set_server_cert(builder.data, cert_path);
}

void
wfd_config_builder_set_server_document_root(
    struct wfd_config_builder builder,
    char const * document_root)
{
    builder.vtable->set_server_document_root(builder.data, document_root);
}

bool
wfd_config_builder_add_auth_provider(
    struct wfd_config_builder builder,
    char const * provider,
    struct wfd_settings * settings)
{
    return builder.vtable->add_auth_provider(builder.data, provider, settings);
}

bool
wfd_config_builder_add_filesystem(
    struct wfd_config_builder builder,
    char const * name,
    char const * mount_point)
{
    return builder.vtable->add_filesystem(builder.data, name, mount_point);
}

bool
wfd_config_builder_set_logger(
    struct wfd_config_builder builder,
    char const * provider,
    int level,
    struct wfd_settings * settings)
{
    return builder.vtable->set_logger(builder.data, provider, level, settings);
}



