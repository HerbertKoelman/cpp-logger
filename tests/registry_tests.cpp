/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include <unistd.h>
#include <syslog.h>
#include "gtest/gtest.h"

/**
 * This checks that a registered logger is instanciated only once.
 */
TEST(registry, unicity_check) {
    logger::logger_ptr err0 = logger::get<logger::stderr_sink>("stderr-test-logger");

    auto logger_count = logger::registry::instance().size();
    logger::logger_ptr err1 = logger::get<logger::stderr_sink>("stderr-test-logger");
    logger::logger_ptr err2 = logger::get<logger::stderr_sink>("stderr-test-logger");
    logger::logger_ptr err3 = logger::get<logger::stderr_sink>("stderr-test-logger");

    EXPECT_NE(err1, nullptr);
    EXPECT_EQ(logger_count, logger::registry::instance().size());
}