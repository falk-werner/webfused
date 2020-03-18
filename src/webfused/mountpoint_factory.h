#ifndef WFD_MOUNTPOINT_FACTORY_H
#define WFD_MOUNTPOINT_FACTORY_H

#include "webfuse/adapter/mountpoint_factory.h"

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_mountpoint_factory;

extern struct wfd_mountpoint_factory *
wfd_mountpoint_factory_create(void);

extern void
wfd_mountpoint_factory_dispose(
    struct wfd_mountpoint_factory * factory);

extern bool
wfd_mountpoint_factory_add_filesystem(
    struct wfd_mountpoint_factory * factory,
    char const * name,
    char const * mount_point);

extern struct wf_mountpoint *
wfd_mountpoint_factory_create_mountpoint(
    char const * filesystem,
    void * user_data);

#ifdef __cplusplus
}
#endif

#endif
