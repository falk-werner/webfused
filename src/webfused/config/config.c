#include "webfused/config/config.h"
#include "webfuse/adapter/server_config.h"
#include "webfused/auth/factory.h"
#include "webfused/auth/authenticator.h"
#include "webfused/mountpoint_factory.h"
#include "webfused/log/manager.h"

#include <stdlib.h>
#include <string.h>

#define WFD_CONFIG_DEFAULT_PORT         (8080)
#define WFD_CONFIG_DEFAULT_VHOSTNAME    ("localhost")

struct wfd_config
{
    struct wf_server_config * server;
    bool has_authenticator;
    struct wfd_authenticator authenticator;
    struct wfd_mountpoint_factory * mountpoint_factory;
    char * user;
    char * group;
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
    char const * provider,
    struct wfd_settings * settings)
{
    bool result = false;
    struct wfd_config * config = data;

    if (!config->has_authenticator)
    {
        result = wfd_authenticator_create(provider, settings, &config->authenticator);
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

static bool
wfd_config_add_filesystem(
    void * data,
    char const * name,
    char const * mount_point)
{
    struct wfd_config * config = data;
    return wfd_mountpoint_factory_add_filesystem(
        config->mountpoint_factory, name, mount_point);
}

static bool
wfd_config_set_logger(
    void * data,
    char const * provider,
    int level,
    struct wfd_settings * settings)
{
    struct wfd_config * config = data;
    return wfd_log_manager_set_logger(provider, level, settings);
}

static void
wfd_config_set_user(
    void * data,
    char const * user,
    char const * group)
{
    struct wfd_config * config = data;
    config->user = strdup(user);
    config->group = strdup(group);
}

static const struct wfd_config_builder_vtable 
wfd_config_vtable_config_builder =
{
    .set_server_vhostname = &wfd_config_set_server_vhostname,
    .set_server_port = &wfd_config_set_server_port,
    .set_server_key = &wfd_config_set_server_key,
    .set_server_cert = &wfd_config_set_server_cert,
    .set_server_document_root = &wfd_config_set_server_document_root,
    .add_auth_provider = &wfd_config_add_auth_provider,
    .add_filesystem = &wfd_config_add_filesystem,
    .set_logger = &wfd_config_set_logger,
    .set_user = &wfd_config_set_user
};

struct wfd_config *
wfd_config_create(void)
{
    struct wfd_config * config = malloc(sizeof(struct wfd_config));

    config->mountpoint_factory = wfd_mountpoint_factory_create();
    config->has_authenticator = false;
    config->server = wf_server_config_create();
    wf_server_config_set_vhostname(config->server, WFD_CONFIG_DEFAULT_VHOSTNAME);
    wf_server_config_set_port(config->server, WFD_CONFIG_DEFAULT_PORT);
    wf_server_config_set_mountpoint_factory(config->server,
        wfd_mountpoint_factory_create_mountpoint,
        config->mountpoint_factory);
    config->user = NULL;
    config->group = NULL;

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
    wfd_mountpoint_factory_dispose(config->mountpoint_factory);

    free(config->user);
    free(config->group);
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

char const *
wfd_config_get_user(
    struct wfd_config * config)
{
    return config->user;
}

char const *
wfd_config_get_group(
    struct wfd_config * config)
{
    return config->group;
}

