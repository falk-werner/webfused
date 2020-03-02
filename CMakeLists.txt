cmake_minimum_required (VERSION 3.10)
project(webfused VERSION 0.1.0 DESCRIPTION "Webfuse daemon")

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

add_library(userdb STATIC 
	src/userdb/userdb.c
)

target_include_directories(userdb PUBLIC
	${LIBCONFIG_INCLUDE_DIRS}
	${OPENSSL_INCLUDE_DIRS}
	${JANSSON_INCLUDE_DIRS}
)

target_compile_options(userdb PUBLIC ${OPENSSL_CFLAGS_OTHER})


add_executable(webfused
	src/daemon/main.c
)

target_link_libraries(webfused PUBLIC 
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
