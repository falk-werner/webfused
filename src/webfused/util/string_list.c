#include "webfused/util/string_list.h"
#include <stdlib.h>
#include <string.h>

#define WFD_STRING_LIST_INITIAL_CAPACITY 8

void
wfd_string_list_init(
    struct wfd_string_list * list)
{
    list->size = 0;
    list->capacity = WFD_STRING_LIST_INITIAL_CAPACITY;
    list->items = malloc(sizeof(char *) * list->capacity);
}

void
wfd_string_list_init_copy(
    struct wfd_string_list * list,
    struct wfd_string_list const * other)
{
    if (0 < other->size)
    {
        list->size = other->size;
        list->capacity = other->capacity;
        list->items = malloc(sizeof(char *) * list->capacity);

        for(size_t i = 0; i < other->size; i++)
        {
            list->items[i] = strdup(other->items[i]);
        }
    }
    else
    {
        wfd_string_list_init(list);
    }
}

void
wfd_string_list_cleanup(
    struct wfd_string_list * list)
{
    for (size_t i = 0; i < list->size; i++)
    {
        free(list->items[i]);
    }
    free(list->items);
}

void
wfd_string_list_add(
    struct wfd_string_list * list,
    char const * item)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;
        list->items = realloc(list->items, sizeof(char *) * list->capacity);
    }

    list->items[list->size] = strdup(item);
    list->size++;
}
