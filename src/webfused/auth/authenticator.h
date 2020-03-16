#ifndef WFD_AUTH_AUTHENTICATOR_H
#define WFD_AUTH_AUTHENTICATOR_H

#include "webfuse/adapter/authenticate.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void
wfd_authenticator_dispose_fn(
    void * data);

struct wfd_authenticator_vtable
{
    wfd_authenticator_dispose_fn * dispose;
    wf_authenticate_fn * authenticate;
};

struct wfd_authenticator
{
    struct wfd_authenticator_vtable const * vtable;
    void * data;
};

extern void
wfd_authenticator_dispose(
    struct wfd_authenticator authenticator);


#ifdef __cplusplus
}
#endif

#endif
