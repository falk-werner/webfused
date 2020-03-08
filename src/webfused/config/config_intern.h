#ifndef WFD_CONFIG_INTERN_H
#define WFD_CONFIG_INTERN_H

#ifdef _cplusplus
extern "C"
{
#endif

struct wfd_config
{
    char * vhost_name;
    char * server_cert;
    char * server_key;
    char * server_doc_root;
    int port;
};

extern struct wfd_config *
wfd_config_create(void);

#ifdef _cplusplus
}
#endif

#endif
