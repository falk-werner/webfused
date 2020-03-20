#include "userdb/userdb.h"
#include <gtest/gtest.h>

TEST(userdb, load_file)
{
    userdb * db = userdb_create("");

    bool success = userdb_load_file(db, "test_passwd.json");
    ASSERT_TRUE(success);

    ASSERT_TRUE(userdb_check(db, "bob", "secret"));

    userdb_dispose(db);
}

TEST(userdb, load_file_failed_no_file)
{
    userdb * db = userdb_create("");

    bool success = userdb_load_file(db, "non_existing.json");
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, save)
{
    {
        userdb * db = userdb_create("");
        userdb_add(db, "bob", "secret");
        ASSERT_TRUE(userdb_save(db, "/tmp/webfused_test.json"));
        userdb_dispose(db);
    }

    {
        userdb * db = userdb_create("");
        ASSERT_TRUE(userdb_load_file(db, "/tmp/webfused_test.json"));
        ASSERT_TRUE(userdb_check(db, "bob", "secret"));
        userdb_dispose(db);
    }
}


TEST(userdb, add)
{
    userdb * db = userdb_create("");

    userdb_add(db, "bob", "secret");
    ASSERT_TRUE(userdb_check(db, "bob", "secret"));
    ASSERT_FALSE(userdb_check(db, "bob", "i_dont_know"));
    ASSERT_FALSE(userdb_check(db, "anna", "secret"));

    userdb_dispose(db);
}

TEST(userdb, remove)
{
    userdb * db = userdb_create("");
    ASSERT_NE(nullptr, db);

    userdb_add(db, "bob", "secret");
    ASSERT_TRUE(userdb_check(db, "bob", "secret"));

    userdb_remove(db, "bob");
    ASSERT_FALSE(userdb_check(db, "bob", "secret"));

    userdb_dispose(db);
}

TEST(userdb, update)
{
    userdb * db = userdb_create("");
    ASSERT_NE(nullptr, db);

    userdb_add(db, "bob", "secret");
    ASSERT_TRUE(userdb_check(db, "bob", "secret"));

    userdb_add(db, "bob", "new_secret");
    ASSERT_FALSE(userdb_check(db, "bob", "secret"));
    ASSERT_TRUE(userdb_check(db, "bob", "new_secret"));

    userdb_dispose(db);
}

TEST(userdb, load_string)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_TRUE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_no_json)
{
    userdb * db = userdb_create("");
    char const contents[] = "brummni";

    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_invalid_type)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"any-userdb\","
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_invalid_version)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 2,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_missing_type)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_type_not_string)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": 42,"
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_missing_major_version)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_major_version_not_int)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": false,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_missing_minor_version)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_minor_version_not_int)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"minor\": false,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_missing_hash_alg)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_hash_alg_not_string)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": 42"
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_missing_meta)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, load_fail_unsupported_hash)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"brummni\""
        "},"
        "\"users\": {"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_FALSE(success);

    userdb_dispose(db);
}

TEST(userdb, fail_missing_user_salt)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "    \"bob\": {"
        "      \"password_hash\": \"e51e27ce47054feead3d83068d47f2a07307d4877ac67da668ef43e0e466fe8c7b66651af14fdb8d48c51592ef5afa0c63f874d20861c6b9ef8e6513bfcaa330\""
        "    }"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_TRUE(success);

    ASSERT_FALSE(userdb_check(db, "bob", "secret"));
    userdb_dispose(db);
}

TEST(userdb, fail_missing_user_hash)
{
    userdb * db = userdb_create("");
    char const contents[] =
        "{"
        "\"meta\": {"
        "    \"type\": \"wf-userdb\","
        "    \"major\": 1,"
        "    \"minor\": 0,"
        "    \"hash_algorithm\": \"sha512\""
        "},"
        "\"users\": {"
        "    \"bob\": {"
        "      \"salt\": \"b3be6979921edecfea88c50d0d1ec40b7f8c383831b2276c65969ead18e47c03\""
        "    }"
        "}"
        "}"
        ;
    bool success = userdb_load_string(db, contents);
    ASSERT_TRUE(success);

    ASSERT_FALSE(userdb_check(db, "bob", "secret"));
    userdb_dispose(db);
}