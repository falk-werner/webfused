#ifndef WFD_CONFIG_INTERN_H
#define WFD_CONFIG_INTERN_H

#ifdef _cplusplus
extern "C"
{
#endif

extern struct wfd_config *
wfd_config_create(void);

extern void
wfd_config_set_server_vhostname(
    struct wfd_config * config,
    char const * vhost_name);

extern void
wfd_config_set_server_port(
    struct wfd_config * config,
    int port);

extern void
wfd_config_set_server_cert(
    struct wfd_config * config,
    char const * cert);

extern void
wfd_config_set_server_key(
    struct wfd_config * config,
    char const * key);

extern void
wfd_config_set_server_document_root(
    struct wfd_config * config,
    char const * document_root);

#ifdef _cplusplus
}
#endif

#endif
