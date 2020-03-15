#ifndef WFD_CONFIG_H
#define WFD_CONFIG_H

#include "webfused/config/builder.h"

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
struct wf_server_config;

extern struct wfd_config *
wfd_config_create(void);

extern void
wfd_config_dispose(
    struct wfd_config * config);

extern struct wfd_config_builder
wfd_config_get_builder(
    struct wfd_config * config);

extern struct wf_server_config *
wfd_config_get_server_config(
    struct wfd_config * config);

#ifdef __cplusplus
}
#endif

#endif
