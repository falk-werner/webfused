#ifndef WFD_CONFIG_FACTORY_H
#define WFD_CONFIG_FACTORY_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "webfused/config/builder.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern bool
wfd_config_load_file(
    struct wfd_config_builder builder,
    char const * filename);

extern bool
wfd_config_load_string(
    struct wfd_config_builder builder,
    char const * contents);

#ifdef __cplusplus
}
#endif

#endif
