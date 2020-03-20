#include "mock/linux.hpp"
#include "util/wrap.hpp"

extern "C"
{

static webfused_test::ILinux * wfd_MockLinux = nullptr;

WFD_WRAP_FUNC0( wfd_MockLinux, uid_t, getuid);
WFD_WRAP_FUNC1( wfd_MockLinux, struct group *, getgrnam, char const *);
WFD_WRAP_FUNC1( wfd_MockLinux, int, setgid, gid_t);
WFD_WRAP_FUNC2( wfd_MockLinux, int, setgroups, int, gid_t *);
WFD_WRAP_FUNC1( wfd_MockLinux, struct passwd *, getpwnam, char const *);
WFD_WRAP_FUNC1( wfd_MockLinux, int, setuid, uid_t);

}

namespace webfused_test
{

MockLinux::MockLinux()
{
    wfd_MockLinux = this;
}

MockLinux::~MockLinux()
{
    wfd_MockLinux = nullptr;
}

}