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
    ./webfused -m test --port=4711

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
