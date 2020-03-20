#include "webfused/config/factory.h"
#include "webfused/config/config_intern.h"
#include "webfused/config/config.h"
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
wfd_config_read_logger(
    config_t * config,
    struct wfd_config * builder)
{
    bool result = true;

    bool hasLogger = (NULL != config_lookup(config, "log"));
    if (hasLogger)
    {
        char const * provider;
        int rc = config_lookup_string(config, "log.provider", &provider);
        if (CONFIG_TRUE != rc)
        {
            WFD_ERROR("failed to load config: missing log provider");
            result = false;
        }

        char const * level_str;
        if (result)
        {
            rc = config_lookup_string(config, "log.level", &level_str);
            if (CONFIG_TRUE != rc)
            {
                WFD_ERROR("failed to load config: missing log level");
                result = false;
            }
        }

        int level;
        if (result)
        {
            bool success = wfd_log_level_parse(level_str, &level);
            if (!success)
            {
                WFD_ERROR("failed to parse log level: unknown value \'%s\'", level_str);
                result = false;
            }
        }

        if (result)
        {
            config_setting_t * setting = config_lookup(config, "log.settings");
            struct wfd_settings settings;
            wfd_settings_init(&settings, setting);
            result = wfd_config_set_logger(builder, provider, level, &settings);
            wfd_settings_cleanup(&settings);
        }
    }

    return result;
}

static void
wfd_config_read_server(
    config_t * config,
    struct wfd_config * builder)
{
    char const * vhost_name;
    int rc = config_lookup_string(config, "server.vhost_name", &vhost_name);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_set_server_vhostname(builder, vhost_name);
    }

    int port;
    rc = config_lookup_int(config, "server.port", &port);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_set_server_port(builder, port);
    }

    char const * cert;
    rc = config_lookup_string(config, "server.tls.certificate", &cert);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_set_server_cert(builder, cert);
    }

    char const * key;
    rc = config_lookup_string(config, "server.tls.key", &key);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_set_server_key(builder, key);
    }

    char const * doc_root;
    rc = config_lookup_string(config, "server.document_root", &doc_root);
    if (CONFIG_TRUE == rc)
    {
        wfd_config_set_server_document_root(builder, doc_root);
    }
}

static bool
wfd_config_read_authenticator(
    config_setting_t * authenticator,
    struct wfd_config * builder)
{
    bool result = (NULL != authenticator);
    if (!result)
    {
        WFD_ERROR("failed to load config: invalid authentication section");
    }

    char const * provider_name = NULL;
    if (result) 
    {
        int rc = config_setting_lookup_string(authenticator, "provider", &provider_name);
        if (CONFIG_TRUE != rc)
        {
            WFD_ERROR("failed to load config: missing authentication provider");
            result = false;
        }
    }

    struct config_setting_t * settings = NULL;
    if (result)
    {
        settings = config_setting_lookup(authenticator, "settings");
        if (NULL == settings)
        {
            WFD_ERROR("failed to load config: missing authentication settings");
            result = false;
        }
    }

    if (result)
    {
        struct wfd_settings auth_settings;
        wfd_settings_init(&auth_settings, settings);

        result = wfd_config_add_auth_provider(builder, provider_name, &auth_settings);
        wfd_settings_cleanup(&auth_settings);
    }

    return result;
}

static bool
wfd_config_read_authentication(
    config_t * config,
    struct wfd_config * builder)
{
    bool result = true;

    config_setting_t * authentication = config_lookup(config, "authentication");
    if (NULL != authentication)
    {
        int length = config_setting_length(authentication);
        for (int i = 0; (result) && (i < length); i++)
        {
            config_setting_t * authenticator = config_setting_get_elem(authentication, i);
            result = wfd_config_read_authenticator(authenticator, builder);
        }
    }
    
    return result;
}

static bool
wfd_config_read_filesystems(
    config_t * config,
    struct wfd_config * builder)
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

            result = wfd_config_add_filesystem(builder, name, mount_point);
            if (!result)
            {
                break;
            }
        }
    }

    return result;
}

static bool
wfd_config_read_user(
    config_t * config,
    struct wfd_config * builder)
{
    bool result = true;
    
    bool has_user = (NULL != config_lookup(config, "user"));
    if (has_user)
    {
        char const * user;
        {
            int rc = config_lookup_string(config, "user.name", &user);
            if (CONFIG_TRUE != rc)
            {
                WFD_ERROR("failed to load config: missing required user propert: \'name\'");
                result = false;
            }
        }

        char const * group;
        if (result)
        {
            int rc = config_lookup_string(config, "user.group", &group);
            if (CONFIG_TRUE != rc)
            {
                WFD_ERROR("failed to load config: missing required user propert: \'group\'");
                result = false;
            }
        }

        if (result)
        {
            wfd_config_set_user(builder, user, group);
        }
    }

    return result;
}

static struct wfd_config *
wfd_config_load(
    config_t * config)
{
    struct wfd_config * result = wfd_config_create();

    bool success = wfd_config_check_version(config)
        && wfd_config_read_logger(config, result)
        && wfd_config_read_authentication(config, result)
        && wfd_config_read_filesystems(config, result)
        && wfd_config_read_user(config, result)
        ;
    
    if (success)
    {
        wfd_config_read_server(config, result);
    }
    
    if (!success)
    {
        wfd_config_dispose(result);
        result = NULL;
    }

    return result;
}

struct wfd_config *
wfd_config_load_file(
    char const * filename)
{
    struct wfd_config * result = NULL;

    config_t config;
    config_init(&config);
    int rc = config_read_file(&config, filename);
    if (CONFIG_TRUE == rc)
    {
        result = wfd_config_load(&config);
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

struct wfd_config *
wfd_config_load_string(
    char const * contents)
{
    struct wfd_config * result = NULL;

    config_t config;
    config_init(&config);
    int rc = config_read_string(&config, contents);
    if (CONFIG_TRUE == rc)
    {
        result = wfd_config_load(&config);
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
