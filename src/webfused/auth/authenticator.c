#include "webfused/auth/authenticator.h"

void
wfd_authenticator_dispose(
    struct wfd_authenticator authenticator)
{
    authenticator.vtable->dispose(authenticator.data);
}
