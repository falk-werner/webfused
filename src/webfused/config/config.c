#include "webfused/config/config.h"
#include "webfuse/adapter/server_config.h"
#include "webfused/auth/factory.h"
#include "webfused/auth/authenticator.h"

#include <stdlib.h>

#define WFD_CONFIG_DEFAULT_PORT         (8080)
#define WFD_CONFIG_DEFAULT_VHOSTNAME    ("localhost")

struct wfd_config
{
    struct wf_server_config * server;
    bool has_authenticator;
    struct wfd_authenticator authenticator;
};

static void
wfd_config_set_server_vhostname(
    void * data,
    char const * vhost_name)
{
    struct wfd_config * config = data;
    wf_server_config_set_vhostname(config->server, vhost_name);
}

static void
wfd_config_set_server_port(
    void * data,
    int port)
{
    struct wfd_config * config = data;
    wf_server_config_set_port(config->server, port);
}

static void
wfd_config_set_server_key(
    void * data,
    char const * key_path)
{
    struct wfd_config * config = data;
    wf_server_config_set_keypath(config->server, key_path);
}

static void
wfd_config_set_server_cert(
    void * data,
    char const * cert_path)
{
    struct wfd_config * config = data;
    wf_server_config_set_certpath(config->server, cert_path);
}

static void
wfd_config_set_server_document_root(
    void * data,
    char const * document_root)
{
    struct wfd_config * config = data;
    wf_server_config_set_documentroot(config->server, document_root);
}

static bool
wfd_config_add_auth_provider(
    void * data,
    struct wfd_auth_settings * settings)
{
    bool result = false;
    struct wfd_config * config = data;

    if (!config->has_authenticator)
    {
        result = wfd_authenticator_create(settings, &config->authenticator);
        if (result)
        {
            wf_server_config_add_authenticator(
                config->server, 
                wfd_authenticator_get_type(config->authenticator), 
                config->authenticator.vtable->authenticate,
                config->authenticator.data);

            config->has_authenticator = true;
        }
    }

    return result;
}


static const struct wfd_config_builder_vtable 
wfd_config_vtable_config_builder =
{
    .set_server_vhostname = &wfd_config_set_server_vhostname,
    .set_server_port = &wfd_config_set_server_port,
    .set_server_key = &wfd_config_set_server_key,
    .set_server_cert = &wfd_config_set_server_cert,
    .set_server_document_root = &wfd_config_set_server_document_root,
    .add_auth_provider = &wfd_config_add_auth_provider
};

struct wfd_config *
wfd_config_create(void)
{
    struct wfd_config * config = malloc(sizeof(struct wfd_config));

    config->server = wf_server_config_create();
    wf_server_config_set_vhostname(config->server, WFD_CONFIG_DEFAULT_VHOSTNAME);
    wf_server_config_set_port(config->server, WFD_CONFIG_DEFAULT_PORT);

    config->has_authenticator = false;

    return config;
}

void
wfd_config_dispose(
    struct wfd_config * config)
{
    wf_server_config_dispose(config->server);
    if (config->has_authenticator)
    {
        wfd_authenticator_dispose(config->authenticator);
    }

    free(config);
}

struct wfd_config_builder
wfd_config_get_builder(
    struct wfd_config * config)
{
    struct wfd_config_builder builder =
    {
        &wfd_config_vtable_config_builder,
        config
    };

    return builder;
}

struct wf_server_config *
wfd_config_get_server_config(
    struct wfd_config * config)
{
    return config->server;
}

