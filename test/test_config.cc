#include <gtest/gtest.h>
#include "webfused/config/config.h"

TEST(config, server_config)
{
    wfd_config * config = wfd_config_create();
    ASSERT_NE(nullptr, config);

    wfd_config_builder builder = wfd_config_get_builder(config);
    wfd_config_builder_set_server_vhostname(builder, "localhost");
    wfd_config_builder_set_server_port(builder, 8443);
    wfd_config_builder_set_server_key(builder, "/path/to/key.pem");
    wfd_config_builder_set_server_cert(builder, "/path/to/cert.pem");
    wfd_config_builder_set_server_document_root(builder, "/var/www");

    wf_server_config * server_config = wfd_config_get_server_config(config);
    ASSERT_NE(nullptr, server_config);

    wfd_config_dispose(config);
}