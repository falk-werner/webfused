PACKAGES="libfuse lws jansson gtest webfuse"

libfuse_VERSION=3.9.1
libfuse_URL=https://github.com/libfuse/libfuse/archive/fuse-${libfuse_VERSION}.tar.gz
libfuse_MD5=5f7c1062def710d8b60343524a18cc82
libfuse_DIR=libfuse-fuse-${libfuse_VERSION}
libfuse_TYPE=meson

lws_VERSION=4.0.13
lws_URL=https://github.com/warmcat/libwebsockets/archive/v${lws_VERSION}.tar.gz
lws_MD5=6403448e26d302501969afb0e128143b
lws_DIR=libwebsockets-${lws_VERSION}
lws_TYPE=cmake

jansson_VERSION=2.12
jansson_URL=https://github.com/akheron/jansson/archive/v${jansson_VERSION}.tar.gz
jansson_MD5=c4b106528d5ffb521178565de1ba950d
jansson_DIR=jansson-${jansson_VERSION}
jansson_TYPE=cmake

gtest_VERSION=1.10.0
gtest_URL=https://github.com/google/googletest/archive/release-${1.10.0}.tar.gz
gtest_md5=ecd1fa65e7de707cd5c00bdac56022cd
gtest_DIR=googletest-release-${1.10.0}
gtest_TYPE=cmake

webfuse_VERSION=0.5.0
webfuse_URL=https://github.com/falk-werner/webfuse/archive/v${webfuse_VERSION}.tar.gz
webfuse_MD5=535c9f261c90c109a138daa3ee9f95ba
webfuse_DIR=webfuse-${webfuse_VERSION}
webfuse_TYPE=meson
