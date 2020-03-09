#ifndef WFD_CONFIG_H
#define WFD_CONFIG_H

#ifndef __cplusplus
#include <stdbool.h>
#include <stddef.h>
#else
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_config;
struct wfd_auth_settins;
struct wfd_filesystem_info;

extern void wfd_config_dispose(
    struct wfd_config * config);

extern int
wfd_config_get_server_port(
    struct wfd_config * config);

extern char const *
wfd_config_get_server_vhostname(
    struct wfd_config * config);

extern char const *
wfd_config_get_server_cert(
    struct wfd_config * config);

extern bool
wfd_config_is_server_tls_enabled(
    struct wfd_config * config);

extern char const *
wfd_config_get_server_key(
    struct wfd_config * config);

extern char const *
wfd_config_get_server_document_root(
    struct wfd_config * config);

extern char const *
wfd_config_get_auth_provider(
    struct wfd_config * config);

extern struct wfd_auth_settings *
wfd_config_get_auth_settings(
    struct wfd_config * config);

extern size_t
wfd_config_get_filesystem_count(
    struct wfd_config * config);

extern struct wfd_filesystem_info *
wfd_confi_get_filesystem(
    struct wfd_config * config,
    size_t fs_index);


#ifdef __cplusplus
}
#endif

#endif
