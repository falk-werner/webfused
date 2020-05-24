project('webfused', 'c', 'cpp', version: '0.3.0', license: 'LGPL-3.0+')

openssl_dep = dependency('openssl', version: '>=1.1.1')
libconfig_dep = dependency('libconfig', version: '>=1.5')
pam_dep = declare_dependency(link_args: ['-lpam'])

libwebsockets_dep = dependency('libwebsockets', version: '>=4.0.1') 
jansson_dep = dependency('jansson', version: '>=2.11', fallback: ['jansson', 'jansson_dep'])
libfuse_dep = dependency('fuse3', version: '>=3.8.0', fallback: ['fuse3', 'libfuse_dep'])
gtest_dep = dependency('gtest', version: '>=1.10.0', fallback: ['gtest', 'gtest_dep']) 
gmock_main_dep = dependency('gmock_main', version: '>=1.10.0', fallback: ['gtest', 'gmock_main_main_dep'])
webfuse_adapter_dep = dependency('webfuse_adapter', version: '>=0.3.0', fallback: ['webfuse', 'webfuse_adapter_dep'])

inc_dir = include_directories('src')

libuserdb = static_library('userdb',
    'src/userdb/userdb.c',
    include_directories: inc_dir,
    dependencies: [libconfig_dep, openssl_dep, jansson_dep])

libuserdb_dep = declare_dependency(
    include_directories: inc_dir,
    link_with: libuserdb,
    dependencies: [libconfig_dep, openssl_dep, jansson_dep])

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
    include_directories: inc_dir,
    dependencies: [libuserdb_dep, webfuse_adapter_dep, pam_dep],
    install: false)

libwebfused_dep = declare_dependency(
    include_directories: inc_dir,
    link_with: libwebfused,
    dependencies: [libuserdb_dep, webfuse_adapter_dep, pam_dep],
    install: false)

webfused = executable('webfused',
	'src/webfused/main.c',
    include_directories: inc_dir,
    dependencies: [libwebfused_dep],
    install: true)

install_data('etc/webfused.conf', install_dir: '/etc')

