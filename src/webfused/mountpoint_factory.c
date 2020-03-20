#include "webfused/mountpoint_factory.h"
#include "webfused/log/log.h"

#include <webfuse/adapter/mountpoint.h>

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define WFD_FILESYSTEM_DEFAULT_CAPACITY 16

struct wfd_filesystem
{
    char * name;
    char * mount_point;
    bool in_use;
};

struct wfd_mountpoint_factory
{
    struct wfd_filesystem * filesystems;
    size_t capacity;
    size_t count;
};

static struct wfd_filesystem *
wfd_mountpoint_factory_find(
    struct wfd_mountpoint_factory * factory,
    char const * name)
{
    for (size_t i = 0; i < factory->count; i++)
    {
        struct wfd_filesystem * filesystem = &(factory->filesystems[i]);
        if (0 == strcmp(name, filesystem->name))
        {
            return filesystem;
        }
    }

    return NULL;
}

static void 
wfd_mountpoint_factory_release_mountpoint(
    void * user_data)
{
    bool * in_use = user_data;
    *in_use = false;
}

struct wfd_mountpoint_factory *
wfd_mountpoint_factory_create(void)
{
    struct wfd_mountpoint_factory * factory = malloc(sizeof(struct wfd_mountpoint_factory));
    factory->filesystems = malloc(sizeof(struct wfd_filesystem) * WFD_FILESYSTEM_DEFAULT_CAPACITY);
    factory->count = 0;
    factory->capacity = WFD_FILESYSTEM_DEFAULT_CAPACITY;

    return factory;
}

void
wfd_mountpoint_factory_dispose(
    struct wfd_mountpoint_factory * factory)
{
    for(size_t i = 0; i < factory->count; i++)
    {
        struct wfd_filesystem * filesystem = &(factory->filesystems[i]);
        free(filesystem->name);
        free(filesystem->mount_point);
    }

    free(factory->filesystems);
    free(factory);
}

bool
wfd_mountpoint_factory_add_filesystem(
    struct wfd_mountpoint_factory * factory,
    char const * name,
    char const * mount_point)
{
    bool result = (NULL == wfd_mountpoint_factory_find(factory, name));
    if (!result)
    {
        WFD_ERROR("mount_point already defined: \'%s\'", mount_point);
    }

    char * path = NULL;
    if (result)
    {
        mkdir(mount_point, 0755);
        path = realpath(mount_point, NULL);
        if (NULL == path)
        {
            WFD_ERROR("invalid mount_point: \'%s\'", mount_point);
            result = false;
        }
    }

    if (result)
    {
        if (factory->count >= factory->capacity)
        {
            factory->capacity *= 2;
            factory->filesystems = realloc(factory->filesystems, 
                sizeof(struct wfd_filesystem) * factory->capacity);
        }

        struct wfd_filesystem * actual = &(factory->filesystems[factory->count]);
        actual->name = strdup(name);
        actual->mount_point = path;
        actual->in_use = false;
        factory->count++;
    }

    return result;
}

extern struct wf_mountpoint *
wfd_mountpoint_factory_create_mountpoint(
    char const * filesystem,
    void * user_data)
{
    struct wfd_mountpoint_factory * factory = user_data;
    struct wfd_filesystem * fs = wfd_mountpoint_factory_find(factory, filesystem);
    if (NULL == fs)
    {
        WFD_INFO("failed to create mountpoint: filesystem \'%s\' not found", filesystem);
        return NULL;
    }

    if (fs->in_use)
    {
        WFD_INFO("failed to create mountpoint: filesystem \'%s\' already in use", filesystem);
        return NULL;
    }

    fs->in_use = true;
    struct wf_mountpoint * result = wf_mountpoint_create(fs->mount_point);
    wf_mountpoint_set_userdata(result,
        &fs->in_use, &wfd_mountpoint_factory_release_mountpoint);

    WFD_INFO("created mountpoint \'%s\' at path \'%s\'", filesystem, fs->mount_point);
    return result;
}
