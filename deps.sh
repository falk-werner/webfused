PACKAGES="fuse3 libwebsockets jansson gtest webfuse"

fuse_VERSION=3.10.0
fuse3_URL=https://github.com/libfuse/libfuse/archive/fuse-${fuse3_VERSION}.tar.gz
fuse3_MD5=22aec9bc9008eea6b17e203653d1b938
fuse3_DIR=libfuse-fuse-${fuse3_VERSION}
fuse3_TYPE=meson

libwebsockets_VERSION=4.1.3
libwebsockets_URL=https://github.com/warmcat/libwebsockets/archive/v${libwebsockets_VERSION}.tar.gz
libwebsockets_MD5=413cbe790ccb089001f53b2ee167b9c2
libwebsockets_DIR=libwebsockets-${libwebsockets_VERSION}
libwebsockets_TYPE=cmake

jansson_VERSION=2.13.1
jansson_URL=https://github.com/akheron/jansson/archive/v${jansson_VERSION}.tar.gz
jansson_MD5=3d589a62053874893715453a46a32a0e
jansson_DIR=jansson-${jansson_VERSION}
jansson_TYPE=cmake

gtest_VERSION=1.10.0
gtest_URL=https://github.com/google/googletest/archive/release-${gtest_VERSION}.tar.gz
gtest_MD5=ecd1fa65e7de707cd5c00bdac56022cd
gtest_DIR=googletest-release-${gtest_VERSION}
gtest_TYPE=cmake

webfuse_VERSION=0.5.1
webfuse_URL=https://github.com/falk-werner/webfuse/archive/v${webfuse_VERSION}.tar.gz
webfuse_MD5=a2e0e9d24cf7bc2d2273b5c64788adca
webfuse_DIR=webfuse-${webfuse_VERSION}
webfuse_TYPE=meson
