/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include "gtest/gtest.h"

TEST(sink, file_sink) {

    logger::file_sink sink("file descriptor", "app", logger::log_level::info, logger::syslog_facility::default_facility(), stdout);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "file descriptor");
    ::testing::internal::CaptureStdout();
    sink.write(logger::log_level::info, "Hello, %s !", "world");
    std::string output = ::testing::internal::GetCapturedStdout();
    EXPECT_TRUE( output.rfind("[L SUBSYS=file descriptor] Hello, world !")!= std::string::npos);

}

TEST(sink, stdout_sink) {

    logger::stdout_sink sink("stdout", "app", logger::log_level::info);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "stdout");
    ::testing::internal::CaptureStdout();
    sink.write(logger::log_level::info, "Hello, %s !", "world");
    std::string output = ::testing::internal::GetCapturedStdout();
    EXPECT_TRUE( output.rfind("[L SUBSYS=stdout] Hello, world !")!= std::string::npos);

    logger::logger_ptr logger_1 = logger::get<logger::stdout_sink>("stdout", logger::syslog_facility::default_facility() );
    logger::logger_ptr logger_2 = logger::get<logger::stdout_sink>("stdout");

}

TEST(sink, stderr_sink) {

    logger::stderr_sink sink("stderr", "app", logger::log_level::info);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "stderr");
    ::testing::internal::CaptureStderr();
    sink.write(logger::log_level::info, "Hello, %s !", "world");
    std::string output = ::testing::internal::GetCapturedStderr();
    EXPECT_TRUE( output.rfind("[L SUBSYS=stderr] Hello, world !")!= std::string::npos);

    logger::logger_ptr logger_1 = logger::get<logger::stderr_sink>("stderr", logger::syslog_facility::default_facility() );
    logger::logger_ptr logger_2 = logger::get<logger::stderr_sink>("stderr");
}

TEST(sink, syslog_sink) {

    logger::syslog_sink sink("syslog", "app", logger::log_level::info);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "syslog");


    logger::logger_ptr logger_1 = logger::get<logger::syslog_sink>("syslog", "local0", 0);
    logger::logger_ptr logger_2 = logger::get<logger::syslog_sink>("syslog");

}
