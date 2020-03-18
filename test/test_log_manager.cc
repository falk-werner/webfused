#include "webfused/log/manager.h"
#include "webfused/log/log.h"

#include <gtest/gtest.h>
#include <stddef.h>

TEST(log_manager, set_logger_fail_unknown_provider)
{
    ASSERT_FALSE(wfd_log_manager_set_logger("unknown", WFD_LOGLEVEL_ALL, NULL));
}
