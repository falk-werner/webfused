#!/bin/bash

PACKAGES="libfuse lws jansson"

libfuse_VERSION=3.9.1
libfuse_URL=https://github.com/libfuse/libfuse/archive/fuse-${libfuse_VERSION}.tar.gz
libfuse_MD5=5f7c1062def710d8b60343524a18cc82
libfuse_DIR=libfuse-fuse-${libfuse_VERSION}
libfuse_TYPE=meson

lws_VERSION=3.2.0
lws_URL=https://github.com/warmcat/libwebsockets/archive/v${lws_VERSION}.tar.gz
lws_MD5=1d06f5602604e67e6f50cef9857c6b0c
lws_DIR=libwebsockets-${lws_VERSION}
lws_TYPE=cmake

jansson_VERSION=2.12
jansson_URL=https://github.com/akheron/jansson/archive/v${jansson_VERSION}.tar.gz
jansson_MD5=c4b106528d5ffb521178565de1ba950d
jansson_DIR=jansson-${jansson_VERSION}
jansson_TYPE=cmake

set -e

install_package() {
    local PACKAGE="$1"
    local PACKAGE_VERSION="${PACKAGE}_VERSION"
    local PACKAGE_URL="${PACKAGE}_URL"
    local PACKAGE_MD5="${PACKAGE}_MD5"
    local PACKAGE_TYPE="${PACKAGE}_TYPE"
    local PACKAGE_DIR="${PACKAGE}_DIR"
    local FILENAME="${PACKAGE}_${!PACKAGE_VERSION}.tar.gz"

    wget "${!PACKAGE_URL}" -O "${FILENAME}"
    md5sum -c <(echo "${!PACKAGE_MD5}" "${FILENAME}")
    tar -xf ${FILENAME}

    case "${!PACKAGE_TYPE}" in
    meson)
        cd "${!PACKAGE_DIR}"
        meson build
        cd build
        ninja
        ninja install
        cd ../..
        ;;
    cmake)
        mkdir "${!PACKAGE_DIR}/build"
        cd "${!PACKAGE_DIR}/build"
        cmake ..
        make
        make install
        cd ../..
        ;;
    *)
        echo "error: unknown package type ${!PACKAGE_TYPE}"
        exit 1
        ;;
    esac
}


CURRENT_DIR=$(pwd)
WORKING_DIR=$(mktemp -d /tmp/install_deps_XXXXXX)
cd ${WORKING_DIR}

for package in ${PACKAGES} ; do
    install_package $package
done

cd ${CURRENT_DIR}
#rm -rf ${WORKING_DIR}
