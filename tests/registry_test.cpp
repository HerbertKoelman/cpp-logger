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

TEST(registry, stderr_sink) {
    logger::logger_ptr err = logger::get<logger::stderr_sink>("stderr-test-logger");

    EXPECT_NE(err, nullptr);
    EXPECT_EQ(err->name(), "stderr-test-logger");

    ::testing::internal::CaptureStderr();

    err->info("stderr sink test (version: %s)", logger::cpp_logger_version());

    std::string output = ::testing::internal::GetCapturedStderr();

	EXPECT_TRUE( output.rfind("[L SUBSYS=stderr-test-logger] stderr sink test")!= std::string::npos);
}

TEST(registry, stdout_sink) {
    logger::logger_ptr out = logger::get<logger::stdout_sink>("stdout-test-logger");

	EXPECT_NE(out, nullptr);
	EXPECT_EQ(out->name(), "stdout-test-logger");

	::testing::internal::CaptureStdout();
	out->info("stdout sink test (version: %s)", logger::cpp_logger_version());
	std::string output = ::testing::internal::GetCapturedStdout();

	EXPECT_TRUE( output.rfind("[L SUBSYS=stdout-test-logger] stdout sink test")!= std::string::npos);
}

TEST(registry, syslog_sink) {
    logger::logger_ptr err = logger::get<logger::syslog_sink>("syslog-test-logger");
    EXPECT_NE(err, nullptr);
    EXPECT_EQ(err->name(), "syslog-test-logger");
}
