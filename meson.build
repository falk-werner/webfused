project('webfused', 'c', 'cpp', version: '0.7.0', license: 'LGPL-3.0+',
    default_options: ['c_std=gnu99', 'cpp_std=gnu++14'])

without_tests = get_option('without_tests')
without_userdb = get_option('without_userdb')

c_compiler = meson.get_compiler('c')

libconfig_dep = dependency('libconfig', version: '>=1.5')
pam_dep = c_compiler.find_library('pam')

libwebsockets_dep = dependency('libwebsockets', version: '>=4.0.0')
libfuse_dep = dependency('fuse3', version: '>=3.1.0')

webfuse_adapter_dep = dependency('webfuse', version: '>=0.7.0')

inc_dir = include_directories('src')

if not without_userdb
    openssl_dep = dependency('openssl', version: '>=1.1.1')
    jansson_dep = dependency('jansson', version: '>=2.7')

    libuserdb = static_library('userdb',
        'src/userdb/userdb_openssl.c',
        include_directories: inc_dir,
        dependencies: [openssl_dep, jansson_dep])

        libuserdb_dep = declare_dependency(
            include_directories: inc_dir,
            link_with: libuserdb,
            dependencies: [openssl_dep, jansson_dep])
else
    libuserdb = static_library('userdb',
        'src/userdb/userdb_none.c',
        include_directories: inc_dir)

        libuserdb_dep = declare_dependency(
            include_directories: inc_dir,
            link_with: libuserdb)
endif

libwebfused = static_library('webfused',
	'src/webfused/daemon.c',
	'src/webfused/mountpoint_factory.c',
	'src/webfused/change_user.c',
	'src/webfused/config/config.c',
	'src/webfused/config/factory.c',
	'src/webfused/config/settings.c',
	'src/webfused/auth/authenticator.c',
	'src/webfused/auth/factory.c',
	'src/webfused/auth/file_authenticator.c',
	'src/webfused/auth/pam_authenticator.c',
	'src/webfused/log/log.c',
	'src/webfused/log/logger.c',
	'src/webfused/log/manager.c',
	'src/webfused/log/stderr_logger.c',
	'src/webfused/log/syslog_logger.c',
    'src/webfused/util/string_list.c',
    include_directories: inc_dir,
    dependencies: [libuserdb_dep, webfuse_adapter_dep, libconfig_dep, pam_dep],
    install: false)

libwebfused_dep = declare_dependency(
    include_directories: inc_dir,
    link_with: libwebfused,
    dependencies: [libuserdb_dep, webfuse_adapter_dep, libconfig_dep, pam_dep])

webfused = executable('webfused',
	'src/webfused/main.c',
    include_directories: inc_dir,
    dependencies: [libwebfused_dep],
    install: true)

install_data('etc/webfused.conf', install_dir: '/etc')


if not without_tests

gtest_dep = dependency('gtest', version: '>=1.10.0', fallback: ['gtest', 'gtest_dep'])
gmock_main_dep = dependency('gmock_main', version: '>=1.10.0', fallback: ['gtest', 'gmock_main_dep'])

webfused_conf    = configure_file(input: 'etc/webfused.conf'    , output: 'webfused.conf'   , copy: true)
invalid_conf     = configure_file(input: 'test/invalid.conf'    , output: 'invalid.conf'    , copy: true)
test_passwd_json = configure_file(input: 'test/test_passwd.json', output: 'test_passwd.json', copy: true)

alltests_sources = [
    'test/mock/config_builder.cc',
    'test/mock/logger.cc',
    'test/mock/credentials.cc',
    'test/mock/settings.cc',
    'test/mock/pam.cc',
    'test/mock/libconfig.cc',
    'test/mock/linux.cc',
    'test/mock/server.cc',
    'test/config/configfile.cc',
    'test/config/configfile_version.cc',
    'test/config/configfile_server.cc',
    'test/config/configfile_auth.cc',
    'test/config/configfile_filesystem.cc',
    'test/config/configfile_log.cc',
    'test/config/configfile_user.cc',
    'test/config/config.cc',
    'test/config/settings.cc',
    'test/auth/factory.cc',
    'test/auth/pam_authenticator.cc',
    'test/log/log.cc',
    'test/log/log_manager.cc',
    'test/log/stderr_logger.cc',
    'test/log/syslog_logger.cc',
    'test/util/string_list.cc',
    'test/daemon.cc',
    'test/change_user.cc',
    'test/mountpoint_factory.cc'
]

if not without_userdb
    alltests_sources += [
        'test/auth/file_authenticator.cc',
        'test/userdb.cc'
    ]
endif

alltests = executable('alltests',
    alltests_sources,
    include_directories: ['src', 'test'],
    link_args: [
        '-Wl,--wrap=wf_credentials_type',
        '-Wl,--wrap=wf_credentials_get',

        '-Wl,--wrap=wf_server_create',

        '-Wl,--wrap=wfd_settings_get_string',
        '-Wl,--wrap=wfd_settings_get_string_or_default',
        '-Wl,--wrap=wfd_settings_get_bool',

        '-Wl,--wrap=wfd_config_create',
        '-Wl,--wrap=wfd_config_dispose',
        '-Wl,--wrap=wfd_config_set_server_vhostname',
        '-Wl,--wrap=wfd_config_set_server_port',
        '-Wl,--wrap=wfd_config_set_server_key',
        '-Wl,--wrap=wfd_config_set_server_cert',
        '-Wl,--wrap=wfd_config_set_server_document_root',
        '-Wl,--wrap=wfd_config_add_auth_provider',
        '-Wl,--wrap=wfd_config_add_filesystem',
        '-Wl,--wrap=wfd_config_set_logger',
        '-Wl,--wrap=wfd_config_set_user',

        '-Wl,--wrap=pam_start',
        '-Wl,--wrap=pam_end',
        '-Wl,--wrap=pam_strerror',
        '-Wl,--wrap=pam_authenticate',
        '-Wl,--wrap=pam_acct_mgmt',

        '-Wl,--wrap=config_setting_get_elem',

        '-Wl,--wrap=getuid',
        '-Wl,--wrap=getgrnam',
        '-Wl,--wrap=setgid',
        '-Wl,--wrap=setgroups',
        '-Wl,--wrap=getpwnam',
        '-Wl,--wrap=setuid'
    ],
    dependencies: [
        libwebfused_dep,
        gtest_dep,
        gmock_main_dep])

test('alltests', alltests)

endif
