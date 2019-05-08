/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include "gtest/gtest.h"

TEST(sink, file_sink) {

    logger::file_sink sink("file descriptor", "app", logger::log_level::info, stdout);

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

    auto pos = output.rfind("[L SUBSYS");
    EXPECT_EQ("[L SUBSYS=stdout] Hello, world !\n", ( pos != std::string::npos ? output.substr(pos) : "pattern \"[L SUBSYS\" not found"));

    logger::logger_ptr logger_2 = logger::get<logger::stdout_sink>("sink_stdout_sink_test");

}

TEST(sink, stdout_sink_default) {

    logger::stdout_sink sink;

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "default");

    ::testing::internal::CaptureStdout();
    sink.write(logger::log_level::info, "Hello, %s !", "world");
    std::string output = ::testing::internal::GetCapturedStdout();

    auto pos = output.rfind("[L SUBSYS");
    EXPECT_EQ("[L SUBSYS=default] Hello, world !\n", ( pos != std::string::npos ? output.substr(pos) : "pattern \"[L SUBSYS\" not found"));

}

TEST(sink, stderr_sink) {

    logger::stderr_sink sink("stderr", "app", logger::log_level::info);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "stderr");

    ::testing::internal::CaptureStderr();
    sink.write(logger::log_level::info, "Hello, %s !", "world");
    std::string output = ::testing::internal::GetCapturedStderr();

    auto pos = output.rfind("[L SUBSYS");
    EXPECT_EQ("[L SUBSYS=stderr] Hello, world !\n", ( pos != std::string::npos ? output.substr(pos) : "pattern \"[L SUBSYS\" not found"));

    auto test_case_meta_data = ::testing::UnitTest::GetInstance();
    auto current_test_case = test_case_meta_data->current_test_case();
    auto current_test_info = test_case_meta_data->current_test_info();

    logger::logger_ptr logger_2 = logger::get<logger::stderr_sink>(std::string(current_test_case->name()) + "." + current_test_info->name());
    logger_2->warning("New output format...");
}

TEST(sink, stderr_sink_default) {
    logger::stderr_sink sink;

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "default");

    ::testing::internal::CaptureStderr();
    sink.write(logger::log_level::info, "Hello, %s !", "world");
    std::string output = ::testing::internal::GetCapturedStderr();

    auto pos = output.rfind("[L SUBSYS");
    EXPECT_EQ("[L SUBSYS=default] Hello, world !\n", ( pos != std::string::npos ? output.substr(pos) : "pattern \"[L SUBSYS\" not found"));
}

TEST(sink, syslog_sink) {

    logger::syslog_sink sink("syslog", "app", logger::log_level::info);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "syslog");


    logger::logger_ptr logger_1 = logger::get<logger::syslog_sink>("syslog", logger::syslog::user_facility, 0);
    logger_1->info("Hello, world ! (cpp-logger version %s", logger::cpp_logger_version());
    logger::logger_ptr logger_2 = logger::get<logger::syslog_sink>("syslog");
}
