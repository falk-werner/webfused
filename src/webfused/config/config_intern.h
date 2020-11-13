#ifndef WFD_CONFIG_INTERN_H
#define WFD_CONFIG_INTERN_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_settings;
struct wfd_config;
struct wfd_string_list;

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
wfd_config_set_server_key(
    struct wfd_config * config,
    char const * key_path);

extern void
wfd_config_set_server_cert(
    struct wfd_config * config,
    char const * cert_path);

extern void
wfd_config_set_server_document_root(
    struct wfd_config * config,
    char const * document_root);

extern bool
wfd_config_add_auth_provider(
    struct wfd_config * config,
    char const * provider,
    struct wfd_settings * settings);

extern bool
wfd_config_add_filesystem(
    struct wfd_config * config,
    char const * name,
    char const * mount_point,
    struct wfd_string_list const * mount_options);

extern bool
wfd_config_set_logger(
    struct wfd_config * config,
    char const * provider,
    int level,
    struct wfd_settings * settings);

extern void
wfd_config_set_user(
    struct wfd_config * config,
    char const * user,
    char const * group);

#ifdef __cplusplus
}
#endif

#endif
