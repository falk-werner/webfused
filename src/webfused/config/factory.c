#include "webfused/config/factory.h"
#include "webfused/config/settings_intern.h"
#include "webfused/log/log.h"

#include <libconfig.h>
#include <stdlib.h>
#include <stdbool.h>


#if ((LIBCONFIG_VER_MAJOR != 1) || (LIBCONFIG_VER_MINOR < 5))
#error "linconfig 1.5 or higher needed"
#endif


#define WFD_CONFIG_VERSION_MAJOR 1
#define WFD_CONFIG_VERSION_MINOR 0

static bool
wfd_config_check_version(
    config_t * config)
{
    int version_major;
    int rc = config_lookup_int(config, "version.major", &version_major);
    if (CONFIG_TRUE != rc)
    {
        WFD_ERROR("failed to load config: missing version.major");
        return false;
    }

    if (WFD_CONFIG_VERSION_MAJOR != version_major)
    {
        WFD_ERROR("failed to load config: " 
            "incompatible versions: expected %d, but war %d",
            WFD_CONFIG_VERSION_MAJOR, version_major);
        return false;
    }

    int version_minor;
    rc = config_lookup_int(config, "version.minor", &version_minor);
    if (CONFIG_TRUE != rc)
    {
        WFD_ERROR("failed to load config: missing version.minor");
        return false;
    }

    if (WFD_CONFIG_VERSION_MINOR < version_minor)
    {
        WFD_WARN("newer config detected: some features might be disabled");
    }
    else if (WFD_CONFIG_VERSION_MINOR > version_minor)
    {
        WFD_INFO("old config detected: some features might use default values");
    }

    return true;
}

static bool
wfd_config_read_server(
    config_t * config,
    struct wfd_config_builder builder)
{
    char const * vhost_name;
    int rc = config_lookup_string(config, "server.vhost_name", &vhost_name);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_builder_set_server_vhostname(builder, vhost_name);
    }

    int port;
    rc = config_lookup_int(config, "server.port", &port);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_builder_set_server_port(builder, port);
    }

    char const * cert;
    rc = config_lookup_string(config, "server.tls.certificate", &cert);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_builder_set_server_cert(builder, cert);
    }

    char const * key;
    rc = config_lookup_string(config, "server.tls.key", &key);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_builder_set_server_key(builder, key);
    }

    char const * doc_root;
    rc = config_lookup_string(config, "server.document_root", &doc_root);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_builder_set_server_document_root(builder, doc_root);
    }

    return true;
}

static bool
wfd_config_read_authentication(
    config_t * config,
    struct wfd_config_builder builder)
{
    bool result = true;

    bool hasAuthentication = (NULL != config_lookup(config, "authentication"));
    if (hasAuthentication)
    {
        char const * provider_name = NULL;
        {
            int rc = config_lookup_string(config, "authentication.provider", &provider_name);
            if (CONFIG_TRUE != rc)
            {
                WFD_ERROR("missing authentication provider");
                result = false;
            }
        }

        struct config_setting_t * settings = NULL;
        if (result)
        {
            settings = config_lookup(config, "authentication.settings");
            if (NULL == settings)
            {
                WFD_ERROR("missing authentication settings");
                result = false;
            }
        }

        if (result)
        {
            struct wfd_settings auth_settings;
            wfd_settings_init(&auth_settings, settings);

            result = wfd_config_builder_add_auth_provider(builder, provider_name, &auth_settings);
            wfd_settings_cleanup(&auth_settings);
        }
    }
    
    return result;
}

static bool
wfd_config_read_filesystems(
    config_t * config,
    struct wfd_config_builder builder)
{
    bool result = true;
    config_setting_t * filesystems = config_lookup(config, "filesystems");
    if (NULL != filesystems)
    {
        int length = config_setting_length(filesystems);
        for (int i = 0; i < length; i++)
        {
            config_setting_t * fs = config_setting_get_elem(filesystems, i);
            if (NULL == fs)
            {
                WFD_ERROR("failed to load config: invalid filesystem section");
                result = false;
                break;
            }

            char const * name;
            int rc = config_setting_lookup_string(fs, "name", &name);
            if (rc != CONFIG_TRUE)
            {
                WFD_ERROR("failed to load config: missing required filesystem property \'name\'");
                result = false;
                break;
            }

            char const * mount_point;
            rc = config_setting_lookup_string(fs, "mount_point", &mount_point);
            if (rc != CONFIG_TRUE)
            {
                WFD_ERROR("failed to load config: missing required filesystem property \'mount_point\'");
                result = false;
                break;
            }

            result = wfd_config_builder_add_filesystem(builder, name, mount_point);
            if (!result)
            {
                break;
            }
        }
    }

    return result;
}

static bool
wfd_config_load(
    struct wfd_config_builder builder,
    config_t * config)
{

    bool result = wfd_config_check_version(config)
        && wfd_config_read_server(config, builder)
        && wfd_config_read_authentication(config, builder)
        && wfd_config_read_filesystems(config, builder)
        ;

    return result;
}

bool
wfd_config_load_file(
    struct wfd_config_builder builder,
    char const * filename)
{
    bool result = false;

    config_t config;
    config_init(&config);
    int rc = config_read_file(&config, filename);
    if (CONFIG_TRUE == rc)
    {
        result = wfd_config_load(builder, &config);
    }
    else
    {
        WFD_ERROR("failed to load config: %s: %d: %s", 
            config_error_file(&config),
            config_error_line(&config),
            config_error_text(&config));
    }
    config_destroy(&config);
    

    return result;
}

bool
wfd_config_load_string(
    struct wfd_config_builder builder,
    char const * contents)
{
    bool result = false;

    config_t config;
    config_init(&config);
    int rc = config_read_string(&config, contents);
    if (CONFIG_TRUE == rc)
    {
        result = wfd_config_load(builder, &config);
    }
    else
    {
        WFD_ERROR("failed to load config: %d: %s", 
            config_error_line(&config),
            config_error_text(&config));
    }
    config_destroy(&config);
    

    return result;
}
