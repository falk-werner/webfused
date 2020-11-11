#include "userdb.h"
#include <stddef.h>

struct userdb * userdb_create(
    char const * pepper)
{
    (void) pepper;
    return NULL;
}

void userdb_dispose(struct userdb * db)
{
    (void) db;
}

bool userdb_save(
    struct userdb * db,
    char const * filename)
{
    (void) db;
    (void) filename;

    return false;
}

bool userdb_load_file(
    struct userdb * db,
    char const * filename)
{
    (void) db;
    (void) filename;

    return false;
}

bool userdb_load_string(
    struct userdb * db,
    char const * contents)
{
    (void) db;
    (void) contents;

    return false;
}

void userdb_add(
    struct userdb * db,
    char const * username,
    char const * password)
{
    (void) db;
    (void) username;
    (void) password;
}

void userdb_remove(
    struct userdb * db,
    char const * user)
{
    (void) db;
    (void) user;
}

bool userdb_check(
    struct userdb * db,
    char const * username,
    char const * password)
{
    (void) db;
    (void) username;
    (void) password;

    return false;
}
