#ifndef WFD_AUTH_AUTHENTICATOR_H
#define WFD_AUTH_AUTHENTICATOR_H

#include "webfuse/authenticate.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void
wfd_authenticator_dispose_fn(
    void * data);

typedef char const *
wfd_authenticator_get_type_fn(
    void * data);

struct wfd_authenticator_vtable
{
    wfd_authenticator_dispose_fn * dispose;
    wf_authenticate_fn * authenticate;
    wfd_authenticator_get_type_fn * get_type;
};

struct wfd_authenticator
{
    struct wfd_authenticator_vtable const * vtable;
    void * data;
};

extern void
wfd_authenticator_dispose(
    struct wfd_authenticator authenticator);

extern bool
wfd_authenticator_authenticate(
    struct wfd_authenticator authenticator,
    struct wf_credentials * credentials);

extern char const * 
wfd_authenticator_get_type(
    struct wfd_authenticator authenticator);

#ifdef __cplusplus
}
#endif

#endif
