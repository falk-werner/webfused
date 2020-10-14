#!/bin/bash

. deps.sh

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
        meson compile -C build
        sudo meson install -C build
        # DESTDIR=$DESTDIR ninja install
        cd ..
        ;;
    cmake)
        mkdir "${!PACKAGE_DIR}/build"
        cd "${!PACKAGE_DIR}/build"
        cmake ..
        make
        sudo make install
        # DESTDIR=$DESTDIR make install
        cd ../..
        ;;
    *)
        echo "error: unknown package type ${!PACKAGE_TYPE}"
        exit 1
        ;;
    esac
}

#DESTDIR=$(realpath .)/.deps
#rm -rf .${DESTDIR}
#mkdir ${DESTDIR}

CURRENT_DIR=$(pwd)
WORKING_DIR=$(mktemp -d /tmp/install_deps_XXXXXX)
cd ${WORKING_DIR}

for package in ${PACKAGES} ; do
    install_package $package
done

cd ${CURRENT_DIR}
#rm -rf ${WORKING_DIR}
