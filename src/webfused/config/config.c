#include "webfused/config/config.h"
#include "webfused/config/config_intern.h"

#include <stdlib.h>
#include <string.h>

#define WFD_CONFIG_DEFAULT_PORT         (8080)
#define WFD_CONFIG_DEFAULT_VHOSTNAME    ("localhost")

struct wfd_config
{
    char * vhost_name;
    char * server_cert;
    char * server_key;
    char * server_doc_root;
    int port;
};

extern struct wfd_config *
wfd_config_create(void)
{
    struct wfd_config * config = malloc(sizeof(struct wfd_config));
    config->port = WFD_CONFIG_DEFAULT_PORT;
    config->vhost_name = strdup(WFD_CONFIG_DEFAULT_VHOSTNAME);
    config->server_key = NULL;
    config->server_cert = NULL;
    config->server_doc_root = NULL;

    return config;
}

void wfd_config_dispose(
    struct wfd_config * config)
{
    free(config->vhost_name);
    free(config->server_cert);
    free(config->server_key);
    free(config->server_doc_root);
    free(config);
}

int
wfd_config_get_server_port(
    struct wfd_config * config)
{
    return config->port;
}

void
wfd_config_set_server_port(
    struct wfd_config * config,
    int port)
{
    config->port = port;
}

char const *
wfd_config_get_server_vhostname(
    struct wfd_config * config)
{
    return config->vhost_name;
}

void
wfd_config_set_server_vhostname(
    struct wfd_config * config,
    char const * vhost_name)
{
    free(config->vhost_name);
    config->vhost_name = strdup(vhost_name);
}

bool
wfd_config_is_server_tls_enabled(
    struct wfd_config * config)
{
    return ((NULL != config->server_key) 
        && (NULL != config->server_cert));
}

char const *
wfd_config_get_server_cert(
    struct wfd_config * config)
{
    return config->server_cert;
}

void
wfd_config_set_server_cert(
    struct wfd_config * config,
    char const * cert)
{
    free(config->server_cert);
    config->server_cert = strdup(cert);
}

char const *
wfd_config_get_server_key(
    struct wfd_config * config)
{
    return config->server_key;
}

void
wfd_config_set_server_key(
    struct wfd_config * config,
    char const * key)
{
    free(config->server_key);
    config->server_key = strdup(key);
}

char const *
wfd_config_get_server_document_root(
    struct wfd_config * config)
{
    return config->server_doc_root;
}

void
wfd_config_set_server_document_root(
    struct wfd_config * config,
    char const * document_root)
{
    free(config->server_doc_root);
    config->server_doc_root = strdup(document_root);
}

char const *
wfd_config_get_auth_provider(
    struct wfd_config * config)
{
    return NULL;
}

struct wfd_auth_settings *
wfd_config_get_auth_settings(
    struct wfd_config * config)
{
    return NULL;
}

size_t
wfd_config_get_filesystem_count(
    struct wfd_config * config)
{
    return 0;
}

struct wfd_filesystem_info *
wfd_confi_get_filesystem(
    struct wfd_config * config,
    size_t fs_index)
{
    return NULL;
}
