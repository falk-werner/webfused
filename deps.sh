PACKAGES="libfuse lws jansson gtest webfuse"

libfuse_VERSION=3.10.0
libfuse_URL=https://github.com/libfuse/libfuse/archive/fuse-${libfuse_VERSION}.tar.gz
libfuse_MD5=22aec9bc9008eea6b17e203653d1b938
libfuse_DIR=libfuse-fuse-${libfuse_VERSION}
libfuse_TYPE=meson

lws_VERSION=4.1.3
lws_URL=https://github.com/warmcat/libwebsockets/archive/v${lws_VERSION}.tar.gz
lws_MD5=413cbe790ccb089001f53b2ee167b9c2
lws_DIR=libwebsockets-${lws_VERSION}
lws_TYPE=cmake

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

webfuse_VERSION=0.5.0
webfuse_URL=https://github.com/falk-werner/webfuse/archive/v${webfuse_VERSION}.tar.gz
webfuse_MD5=535c9f261c90c109a138daa3ee9f95ba
webfuse_DIR=webfuse-${webfuse_VERSION}
webfuse_TYPE=meson
