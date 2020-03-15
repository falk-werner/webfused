#ifndef WFD_CONFIG_BUILDER_H
#define WFD_CONFIG_BUILDER_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void
wfd_config_builder_set_server_vhostname_fn(
    void * data,
    char const * vhost_name);

typedef void
wfd_config_builder_set_server_port_fn(
    void * data,
    int port);

typedef void
wfd_config_builder_set_server_key_fn(
    void * data,
    char const * key_path);

typedef void
wfd_config_builder_set_server_cert_fn(
    void * data,
    char const * cert_path);

typedef void
wfd_config_builder_set_server_document_root_fn(
    void * data,
    char const * document_root);

struct wfd_config_builder_vtable
{
    wfd_config_builder_set_server_vhostname_fn * set_server_vhostname;
    wfd_config_builder_set_server_port_fn * set_server_port;
    wfd_config_builder_set_server_key_fn * set_server_key;
    wfd_config_builder_set_server_cert_fn * set_server_cert;
    wfd_config_builder_set_server_document_root_fn * set_server_document_root;
};

struct wfd_config_builder
{
    struct wfd_config_builder_vtable const * vtable;
    void * data;
};

extern void
wfd_config_builder_set_server_vhostname(
    struct wfd_config_builder builder,
    char const * vhost_name);

extern void
wfd_config_builder_set_server_port(
    struct wfd_config_builder builder,
    int port);

extern void
wfd_config_builder_set_server_key(
    struct wfd_config_builder builder,
    char const * key_path);

extern void
wfd_config_builder_set_server_cert(
    struct wfd_config_builder builder,
    char const * cert_path);

extern void
wfd_config_builder_set_server_document_root(
    struct wfd_config_builder builder,
    char const * document_root);


#ifdef __cplusplus
}
#endif

#endif