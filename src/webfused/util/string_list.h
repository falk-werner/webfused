#ifndef WFD_UTIL_STRING_LIST_H
#define WFD_UTIL_STRING_LIST_H

#ifndef __cplusplus
#include <stddef.h>
#else
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct wfd_string_list
{
    size_t size;
    size_t capacity;
    char * * items;
};

extern void
wfd_string_list_init(
    struct wfd_string_list * list);

extern void
wfd_string_list_init_copy(
    struct wfd_string_list * list,
    struct wfd_string_list const * other);

extern void
wfd_string_list_cleanup(
    struct wfd_string_list * list);

extern void
wfd_string_list_add(
    struct wfd_string_list * list,
    char const * item);

#ifdef __cplusplus
}
#endif

#endif
