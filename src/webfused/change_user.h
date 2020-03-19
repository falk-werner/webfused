#ifndef WFD_CHANGE_USER_H
#define WFD_CHANGE_USER_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern bool
wfd_change_user(
    char const * user,
    char const * group);

#ifdef __cplusplus
}
#endif

#endif
