cmake_minimum_required (VERSION 3.10)
project(webfused VERSION 0.1.0 DESCRIPTION "Webfuse daemon")

option(WITHOUT_TESTS   "disable unit tests"   OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE3 REQUIRED fuse3)
pkg_check_modules(LWS REQUIRED libwebsockets)
pkg_check_modules(JANSSON REQUIRED jansson)
pkg_check_modules(UUID REQUIRED uuid)
pkg_check_modules(LIBCONFIG REQUIRED libconfig)
pkg_check_modules(OPENSSL REQUIRED openssl)
pkg_check_modules(WEBFUSE REQUIRED libwebfuse-adapter)

add_definitions(-D_FILE_OFFSET_BITS=64)

include_directories(
	"src"
	${WEBFUSE_INCLUDE_DIRS}
)

link_directories(
    ${WEBFUSE_LIBRARY_DIRS}
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(C_WARNINGS -Wall -Wextra)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(coverage)

add_library(userdb STATIC 
	src/userdb/userdb.c
)

target_include_directories(userdb PUBLIC
	${LIBCONFIG_INCLUDE_DIRS}
	${OPENSSL_INCLUDE_DIRS}
	${JANSSON_INCLUDE_DIRS}
)

target_compile_options(userdb PUBLIC ${OPENSSL_CFLAGS_OTHER})

add_library(webfused-static STATIC
	src/webfused/daemon.c
	src/webfused/mountpoint_factory.c
	src/webfused/config/config.c
	src/webfused/config/factory.c
	src/webfused/config/builder.c
	src/webfused/config/auth_settings.c
	src/webfused/auth/authenticator.c
	src/webfused/auth/factory.c
	src/webfused/auth/file_authenticator.c
	src/webfused/log/logger.c
	src/webfused/log/stderr_logger.c
	src/webfused/log/syslog_logger.c
)

add_executable(webfused
	src/webfused/main.c
)

target_link_libraries(webfused PUBLIC
	webfused-static
    userdb
	${LIBCONFIG_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${WEBFUSE_LIBRARIES}
    ${UUID_LIBRARIES}
)

target_compile_options(webfused PUBLIC ${OPENSSL_CFLAGS_OTHER})
install(TARGETS webfused DESTINATION bin)

add_executable(webfuse-passwd
	src/passwd/main.c
)

target_link_libraries(webfuse-passwd PUBLIC 
	userdb
	${OPENSSL_LIBRARIES}
	${JANSSON_LIBRARIES}
)

target_include_directories(webfuse-passwd PUBLIC 
	example/passwd
	example/lib/userdb/include
	${OPENSSL_INCLUDE_DIRS}
	${JANSSON_INCLUDE_DIRS}
)

target_compile_options(webfuse-passwd PUBLIC ${OPENSSL_CFLAGS_OTHER})
install(TARGETS webfuse-passwd DESTINATION bin)

if(NOT WITHOUT_TESTS)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include (CTest)

pkg_check_modules(GTEST gtest_main)
include(GoogleTest)
pkg_check_modules(GMOCK gmock)

add_executable(alltests
	test/mock_config_builder.cc
	test/mock_logger.cc
	test/mock_credentials.cc
	test/mock_auth_settings.cc
    test/test_config_factory.cc
	test/test_config.cc
	test/test_auth_settings.cc
	test/test_auth_factory.cc
	test/test_file_authenticator.cc
	test/test_mountpoint_factory.cc
	test/test_log.cc
)

target_include_directories(alltests PRIVATE
    src
    ${GMOCK_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

target_compile_options(alltests PRIVATE ${GMOCK_CFLAGS} ${GTEST_CFLAGS} "-pthread")

target_link_libraries(alltests PRIVATE
	-Wl,--wrap=wf_credentials_type
	-Wl,--wrap=wf_credentials_get
	-Wl,--wrap=wfd_auth_settings_get_provider
	-Wl,--wrap=wfd_auth_settings_get
	webfused-static
	userdb
	${LIBCONFIG_LIBRARIES}
    ${WEBFUSE_LIBRARIES}
    ${UUID_LIBRARIES}
	${OPENSSL_LIBRARIES}
	${GMOCK_LIBRARIES}
	${GTEST_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

# copy test data
configure_file(etc/webfused.conf webfused.conf COPYONLY)
configure_file(test/invalid.conf invalid.conf COPYONLY)
configure_file(test/test_passwd.json test_passwd.json COPYONLY)

enable_testing()
gtest_discover_tests(alltests TEST_PREFIX alltests:)

add_custom_target(check
    ./alltests)
add_dependencies(check alltests)

add_custom_target(memcheck
    valgrind ./alltests)
add_dependencies(memcheck alltests)

add_custom_target(coverage
    mkdir -p coverage
	COMMAND lcov --initial --capture --directory . --output-file coverage/lcov_base.info --rc lcov_branch_coverage=1
	COMMAND ./alltests
	COMMAND lcov --capture --directory . --output-file coverage/lcov.info --rc lcov_branch_coverage=1
	COMMAND lcov -a coverage/lcov_base.info -a coverage/lcov.info --output-file coverage/lcov.info --rc lcov_branch_coverage=1
	COMMAND lcov --remove coverage/lcov.info '/usr/*' --output-file coverage/lcov.info --rc lcov_branch_coverage=1
	COMMAND lcov --remove coverage/lcov.info '*/test/*' --output-file coverage/lcov.info --rc lcov_branch_coverage=1
)
add_dependencies(coverage alltests)

add_custom_target(coverage-report
	COMMAND genhtml --branch-coverage --highlight --legend coverage/lcov.info --output-directory coverage/report
)
add_dependencies(coverage-report coverage)


endif(NOT WITHOUT_TESTS)
