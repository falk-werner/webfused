# Configuration

A config file is used to configure webfuse daemon.

## Config file

```
version = { major = 1, minor = 1 }

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
        provider = "pam"
        settings:
        {
            service_name = "webfused"
        }
    }
)

filesystems:
(
    {name = "test",  mount_point = "/tmp/webfused", mount_options = () }
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

## Version

The version sections specifies the schema version of the config file.
Currently, there is only one schema version defined: 1.0

## Server

| Setting       | Type   | Default value | Description              |
| ------------- | ------ | ------------- | ------------------------ |
| vhostname     | string | localhost     | Name of the virtual host |
| port          | int    | 8080          | Port number of server    |
| document_root | string | *-empty-*     | Path of HTTP files       |
| tls           | object | *-empty-*     | see below                |

When *document_root* is omitted, no HTTP files are served.

### TLS

| Setting     | Type   | Default value | Description                                 |
| ----------- | ------ | ------------- | ------------------------------------------- |
| certificate | string | *-empty-*     | Path to servers own certificate (.pem file) |
| key         | string | *-empty-*     | Path to servers own private key (.pem file) |

TLS is only activated, when both, *certificate* and *key* are specified.
Otherwise, plain websockes without TLS are used.

## Authentication

| Setting  | Type   | Default value | Description                                     |
| -------- | ------ | ------------- | ----------------------------------------------- |
| provider | string | *-required-*  | Name of the authentication provider (see below) |
| settings | object | *-empty-*     | Provider specific settings (see below)

Currently, the following providers are supported:

- *file*: file based authentication
- *pam*: authentication based on Linux PAM

## File Authenticaton Provider

Allows authentication against a file containing username and password.

| Setting  | Type   | Default value | Description                     |
| -------- | ------ | ------------- | ------------------------------- |
| file     | string | *-required-*  | Path to the authentication file |

## PAM Authenticaton Provider

Allows authentication using Linux PAM.

| Setting      | Type   | Default value | Description            |
| ------------ | ------ | ------------- | ---------------------- |
| service_name | string | webfused      | PAM service identifier |

## Filesystems

Contains a list of file systems that can be provided by webfuse providers.

| Setting     | Type   | Default value | Description                        |
| ----------- | ------ | ------------- | ---------------------------------- |
| name        | string | *-required-*  | Name of the filesystem             |
| mount_point | string | *-required-*  | Local path to mount the filesystem |

## Log

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

### Stderr Logger

This logger does not provide any settings.

### Syslog Logger

| Setting     | Type   | Default value | Description                                |
| ----------- | ------ | ------------- | ------------------------------------------ |
| ident       | string | webfused      | Syslog ident (see syslog documentation)    |
| facility    | string | daemon        | Syslog facility (see syslog documentation) |
| log_pid     | bool   | false         | Add process ID to log messages             |

## User

| Setting | Type   | Default value | Description                     |
| ------- | ------ | ------------- | ------------------------------- |
| name    | string | *-required-*  | Name of the user to switch to.  |
| group   | string | *-required-*  | Name of the group to switch to. |

Webfuse daemon will not run as root. If started as root, webfuse daemon tries to
switch to *user* and *group* provided in config file.

*Note*: user and group are not switched, when webfuse daemon is not started as root.
