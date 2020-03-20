#include <gtest/gtest.h>
#include "webfused/auth/factory.h"
#include "webfused/auth/authenticator.h"
#include "webfused/log/log.h"
#include "mock/logger.hpp"

using ::testing::_;
using ::webfused_test::MockLogger;


TEST(auth_factory, fail_unknown_provider)
{
    MockLogger logger;
    EXPECT_CALL(logger, log(WFD_LOGLEVEL_ERROR, _, _)).Times(1);
    EXPECT_CALL(logger, onclose()).Times(1);

    wfd_authenticator authenticator;
    bool result = wfd_authenticator_create("unknown", NULL, &authenticator);
    ASSERT_FALSE(result);
}