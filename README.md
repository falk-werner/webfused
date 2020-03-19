[![Build Status](https://travis-ci.org/falk-werner/webfused.svg?branch=master)](https://travis-ci.org/falk-werner/webfused)
[![codecov](https://codecov.io/gh/falk-werner/webfused/branch/config_file/graph/badge.svg)](https://codecov.io/gh/falk-werner/webfused)

# webfused

Reference implementation of webfuse daemon.

## Build and run

To install dependecies, see below.

    cd webfused
    mkdir build
    cd build
    cmake ..
    make
    ./webfused -f webfused.conf

## Config file

```
version = { major = 1, minor = 0 }

server:
{
    vhost_name = "localhost"
    port = 8080

    tls:
    {
       certificate = "/etc/webfused/cert.pem"

       key = "/etc/webfused/key.pem"
    }

    document_root = "/var/www"
}

authentication:
(
    {
        provider = "file"
        settings:
        {
            file = "/etc/webfused/passwd"
        }
    }
)

filesystems:
(
    {name = "test",  mount_point = "/tmp/webfused" }
)

log:
{
    provider: "syslog"
    level: "warning"
    settings:
    {
        ident = "webfused"
        facility = "daemon"
        log_pid = true
    }
}

user:
{
    name  = "webfused"
    group = "webfused"
}
```

### Version

The version sections specifies the schema version of the config file.
Currently, there is only one schema version defined: 1.0

### Server

| Setting       | Type   | Default value | Description              |
| ------------- | ------ | ------------- | ------------------------ |
| vhostname     | string | localhost     | Name of the virtual host |
| port          | int    | 8080          | Port number of server    |
| document_root | string | *-empty-*     | Path of HTTP files       |
| tls           | object | *-empty-*     | see below                |

When *document_root* is omitted, no HTTP files are served.

#### TLS

| Setting     | Type   | Default value | Description                                 |
| ----------- | ------ | ------------- | ------------------------------------------- |
| certificate | string | *-empty-*     | Path to servers own certificate (.pem file) |
| key         | string | *-empty-*     | Path to servers own private key (.pem file) |

TLS is only activated, when both, *certificate* and *key* are specified.
Otherwise, plain websockes without TLS are used.

### Authentication

| Setting  | Type   | Default value | Description                                     |
| -------- | ------ | ------------- | ----------------------------------------------- |
| provider | string | *-required-*  | Name of the authentication provider (see below) |
| settings | object | *-empty-*     | Provider specific settings (see below)

Currently, only one provider is specified:

- *file*: file based authentication

### File Authenticaton Provider

Allows authentication against a file containing username and password.

| Setting  | Type   | Default value | Description                     |
| -------- | ------ | ------------- | ------------------------------- |
| file     | string | *-empty-*     | Path to the authentication file |

### Filesystems

Contains a list of file systems that can be provided by webfuse providers.

| Setting     | Type   | Default value | Description                        |
| ----------- | ------ | ------------- | ---------------------------------- |
| name        | string | *-required-*  | Name of the filesystem             |
| mount_point | string | *-required-*  | Local path to mount the filesystem |

### Log

| Setting     | Type   | Default value | Description                            |
| ----------- | ------ | ------------- | -------------------------------------- |
| provider    | string | *-required-*  | Name of log provider (see below)       |
| level       | string | *-required-*  | Log level (see below)                  |
| settings    | object | *-empty-*     | Provider specific settings (see below) |

The following log levels are supported:

- *none*: diabled logging
- *fatal*: log only fatal errors
- *error*: log all kind of errors
- *warn*: log errors and warnings
- *info*: log info messages, warnings and errors
- *debug*: log debug and info messages as well as warnings and errors
- *all*: log all kind of messages

Currently, the following providers are available:

- *stderr*: logs to console error output
- *syslog*: logs to syslog

#### Stderr Logger

This logger does not provide any settings.

#### Syslog Logger

| Setting     | Type   | Default value | Description                                |
| ----------- | ------ | ------------- | ------------------------------------------ |
| ident       | string | webfused      | Syslog ident (see syslog documentation)    |
| facility    | string | daemon        | Syslog facility (see syslog documentation) |
| log_pid     | bool   | false         | Add process ID to log messages             |

### User

| Setting | Type   | Default value | Description                     |
| ------- | ------ | ------------- | ------------------------------- |
| name    | string | *-required-*  | Name of the user to switch to.  |
| group   | string | *-required-*  | Name of the group to switch to. |

Webfuse daemon will not run as root. If started as root, webfuse daemon tries to
switch to *user* and *group* provided in config file.

*Note*: user and group are not switched, when webfuse daemon is not started as root.

## Dependencies

- [webfuse](https://github.com/falk-werner/webfuse)
  - [libfuse](https://github.com/libfuse/libfuse/)
  - [libwebsockets](https://libwebsockets.org/)
  - [jansson](https://github.com/akheron/jansson)
- [openssl](https://www.openssl.org/)
- [libconfig](https://hyperrealm.github.io/libconfig/)
- [Google Test](https://github.com/google/googletest) *(Test only)*

### Installing dependencies

#### libfuse

To install libfuse, meson is needed. Please refer to [meson quick guide](https://mesonbuild.com/Quick-guide.html) for setup instructions.

    wget https://github.com/libfuse/libfuse/archive/fuse-3.9.0.tar.gz -O fuse.tar.gz
    tar -xf fuse.tar.gz
    cd libfuse-fuse-3.9.0
    mkdir .build
    cd .build
    meson ..
    ninja
    sudo ninja install

#### libwebsockets

    wget https://github.com/warmcat/libwebsockets/archive/v3.2.0.tar.gz -O libwebsockets.tar.gz
    tar -xf libwebsockets.tar.gz
    cd libwebsockets-3.2.0
    mkdir .build
    cd .build
    cmake ..
    make
    sudo make install

#### jansson

    wget https://github.com/akheron/jansson/archive/v2.12.tar.gz -O jansson.tar.gz
    tar -xf jansson.tar.gz
    cd jansson-2.12
    mkdir .build
    cd .build
    cmake ..
    make
    sudo make install

#### openssl

    sudo apt update
    sudo install openssl libssl-dev

#### webfuse

    wget https://github.com/falk-werner/webfuse/archive/v0.2.0.tar.gz -O webfuse.tar.gz
    tar -xf webfuse.tar.gz
    cd webfuse-0.2.0
    mkdir .build
    cd .build
    cmake -DWITHOUT_TESTS=ON ..
    make
    sudo make install

#### libconfig

    sudo apt update
    sudo apt install libconfig-dev

#### GoogleTest

Installation of GoogleTest is optional webfuse library, but required to compile tests.

    wget -O gtest-1.10.0.tar.gz https://github.com/google/googletest/archive/release-1.10.0.tar.gz
    tar -xf gtest-1.10.0.tar.gz
    cd googletest-release-1.10.0
    mkdir .build
    cd .build
    cmake ..
    make
    sudo make install
