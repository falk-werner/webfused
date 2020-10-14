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
