#include "webfused/config/factory.h"
#include "webfused/config/config_intern.h"
#include "webfused/config/config.h"
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

static struct wfd_config *
wfd_config_load(config_t * config)
{
    struct wfd_config * result = wfd_config_create();

    bool success = wfd_config_check_version(config);

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
        config_destroy(&config);
    }

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
        config_destroy(&config);
    }

    return result;
}
