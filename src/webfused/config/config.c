#include "webfused/config/config.h"
#include "webfused/config/config_intern.h"
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

void
wfd_config_set_server_vhostname(
    struct wfd_config * config,
    char const * vhost_name)
{
    wf_server_config_set_vhostname(config->server, vhost_name);
}

void
wfd_config_set_server_port(
    struct wfd_config * config,
    int port)
{
    wf_server_config_set_port(config->server, port);
}

void
wfd_config_set_server_key(
    struct wfd_config * config,
    char const * key_path)
{
    wf_server_config_set_keypath(config->server, key_path);
}

void
wfd_config_set_server_cert(
    struct wfd_config * config,
    char const * cert_path)
{
    wf_server_config_set_certpath(config->server, cert_path);
}

void
wfd_config_set_server_document_root(
    struct wfd_config * config,
    char const * document_root)
{
    wf_server_config_set_documentroot(config->server, document_root);
}

bool
wfd_config_add_auth_provider(
    struct wfd_config * config,
    char const * provider,
    struct wfd_settings * settings)
{
    bool result = false;

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

bool
wfd_config_add_filesystem(
    struct wfd_config * config,
    char const * name,
    char const * mount_point)
{
    return wfd_mountpoint_factory_add_filesystem(
        config->mountpoint_factory, name, mount_point);
}

bool
wfd_config_set_logger(
    struct wfd_config * config,
    char const * provider,
    int level,
    struct wfd_settings * settings)
{
    return wfd_log_manager_set_logger(provider, level, settings);
}

void
wfd_config_set_user(
    struct wfd_config * config,
    char const * user,
    char const * group)
{
    config->user = strdup(user);
    config->group = strdup(group);
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

