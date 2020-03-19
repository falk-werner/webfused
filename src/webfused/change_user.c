#include "webfused/change_user.h"
#include "webfused/log/log.h"

#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool
wfd_switch_group(
    char const * group_name)
{
    struct group * group = getgrnam(group_name);
    bool result = (NULL != group);
    if (!result)
    {
        WFD_ERROR("failed to switch group: unknown group \'%s\'", group_name);
    }

    if (result)
    {
        result = (0 != group->gr_gid);
        if (!result)
        {
            WFD_ERROR("failed to switch group: switch to root (gid 0) is not allowed");
        }
    }

    if (result)
    {
        result = (0 == setgid(group->gr_gid));
        if (!result)
        {
            WFD_ERROR("failed to set group id: %s", strerror(errno));
        }
    }

    if (result)
    {
        result = (0 == setgroups(0, NULL));
        if (!result)
        {
            WFD_ERROR("failed to release supplemenatary groups (setgroups): %s", strerror(errno));
        }
    }

    return result;
}

static bool
wfd_switch_user(
    char const * user_name)
{
    struct passwd * user = getpwnam(user_name);
    bool result = (NULL != user);
    if (!result)
    {
        WFD_ERROR("failed to switch user: unknown user \'%s\'", user_name);
    }

    if (result)
    {
        result = (0 != user->pw_uid);
        if (!result)
        {
            WFD_ERROR("failed to switch user: switch to root (uid 0) is not allowed");
        }
    }

    if (result)
    {
        result = (0 == setuid(user->pw_uid));
        if (!result)
        {
            WFD_ERROR("failed to switch user (setuid): %s", strerror(errno));
        }
    }

    return result;
}

bool
wfd_change_user(
    char const * user,
    char const * group)
{
    bool result = true;
    bool const is_root = (0 == getuid());

    if (is_root)
    {
        result = ((NULL != user) || (NULL != group));
        if (!result)
        {
            WFD_ERROR("webfuse daemon cannot be run as root: specify user and group in config");
        }

        if (result)
        {
            result = wfd_switch_group(group);
        }

        if (result)
        {
            result = wfd_switch_user(user);
        }

    }

    return result;
}
