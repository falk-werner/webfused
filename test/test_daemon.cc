#include "webfused/daemon.h"

#include <gtest/gtest.h>
#include <cstdlib>
#include <chrono>
#include <thread>

TEST(daemon, print_usage)
{
    char argv0[] = "daemon";
    char argv1[] = "--help";
    char * argv[] = { argv0, argv1, NULL};

    int exit_code = wfd_daemon_run(2, argv);
    ASSERT_EQ(EXIT_SUCCESS, exit_code);
}

TEST(daemon, print_usage_short)
{
    char argv0[] = "daemon";
    char argv1[] = "-h";
    char * argv[] = { argv0, argv1, NULL};

    int exit_code = wfd_daemon_run(2, argv);
    ASSERT_EQ(EXIT_SUCCESS, exit_code);
}

TEST(daemon, fail_invalid_argument)
{
    char argv0[] = "daemon";
    char argv1[] = "-x";
    char * argv[] = { argv0, argv1, NULL};

    int exit_code = wfd_daemon_run(2, argv);
    ASSERT_EQ(EXIT_FAILURE, exit_code);
}

TEST(daemon, fail_invalid_config_file)
{
    char argv0[] = "daemon";
    char argv1[] = "-f";
    char argv2[] = "invalid.conf";
    char * argv[] = { argv0, argv1, argv2, NULL};

    int exit_code = wfd_daemon_run(3, argv);
    ASSERT_EQ(EXIT_FAILURE, exit_code);
}

void defered_raise()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    raise(SIGINT);
}

TEST(daemon, run)
{
    char argv0[] = "daemon";
    char argv1[] = "-f";
    char argv2[] = "webfused.conf";
    char * argv[] = { argv0, argv1, argv2, NULL};

    std::thread thread(defered_raise);

    int exit_code = wfd_daemon_run(3, argv);
    ASSERT_EQ(EXIT_SUCCESS, exit_code);

    thread.join();
}
