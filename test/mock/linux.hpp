#ifndef WFD_MOCK_LINUX_HPP
#define WFD_MOCK_LINUX_HPP

#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

#include <gmock/gmock.h>

namespace webfused_test
{

class ILinux
{
public:
    virtual ~ILinux() = default;
    virtual uid_t getuid() = 0;
    virtual struct group * getgrnam(char const * name) = 0;
    virtual int setgid(gid_t gid) = 0;
    virtual int setgroups(int size, gid_t * list) = 0;
    virtual struct passwd * getpwnam(char const * name) = 0;
    virtual int setuid(uid_t uid) = 0;
};

class MockLinux: public ILinux
{
public:
    MockLinux();
    ~MockLinux();
    MOCK_METHOD0(getuid, uid_t());
    MOCK_METHOD1(getgrnam, struct group * (char const * name));
    MOCK_METHOD1(setgid, int (gid_t gid));
    MOCK_METHOD2(setgroups, int (int size, gid_t * list));
    MOCK_METHOD1(getpwnam, struct passwd * (char const * name));
    MOCK_METHOD1(setuid, int (uid_t uid));
};

}

#endif
