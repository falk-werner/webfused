#ifndef WFD_AUTHENTICATION_FACTORY_H
#define WFD_AUTHENTICATION_FACTORY_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_authenticator;
struct wfd_auth_settings;

extern bool
wfd_authenticator_create(
    struct wfd_auth_settings * settings,
    struct wfd_authenticator * authenticator);

#ifdef __cplusplus
}
#endif

#endif
