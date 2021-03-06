#include "webfused/auth/authenticator.h"

void
wfd_authenticator_dispose(
    struct wfd_authenticator authenticator)
{
    authenticator.vtable->dispose(authenticator.data);
}

extern bool
wfd_authenticator_authenticate(
    struct wfd_authenticator authenticator,
    struct wf_credentials * credentials)
{
    return authenticator.vtable->authenticate(
        credentials, authenticator.data);
}

char const *
wfd_authenticator_get_type(
    struct wfd_authenticator authenticator)
{
    return authenticator.vtable->get_type(authenticator.data);
}
