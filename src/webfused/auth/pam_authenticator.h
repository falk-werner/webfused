#ifndef WFD_AUTH_PAM_AUTHENTICATOR_H
#define WFD_AUTH_PAM_AUTHENTICATOR_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_authenticator;
struct wfd_settings;

extern bool
wfd_pam_authenticator_create(
    struct wfd_settings * settings,
    struct wfd_authenticator * authenticator);

#ifdef __cplusplus
}
#endif

#endif
