/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include <unistd.h>
#include <syslog.h>
#include "gtest/gtest.h"

TEST(registry, unicity_check) {
    logger::logger_ptr err0 = logger::get<logger::stderr_sink>("stderr-test-logger");

    auto logger_count = logger::registry::instance().size();
    logger::logger_ptr err1 = logger::get<logger::stderr_sink>("stderr-test-logger");
    logger::logger_ptr err2 = logger::get<logger::stderr_sink>("stderr-test-logger");
    logger::logger_ptr err3 = logger::get<logger::stderr_sink>("stderr-test-logger");

    EXPECT_NE(err1, nullptr);
    EXPECT_EQ(logger_count, logger::registry::instance().size());
}

TEST(registry, reset_refgistry) {
    logger::logger_ptr err0 = logger::get<logger::stderr_sink>("stderr-test-logger");

    auto logger_count = logger::registry::instance().size();
    EXPECT_EQ(1, logger_count);

    logger::reset_registry();
    logger_count = logger::registry::instance().size();

    EXPECT_EQ(0, logger_count);

    err0 = logger::get<logger::stderr_sink>("stderr-test-logger-one");
    err0 = logger::get<logger::stderr_sink>("stderr-test-logger-two");

    logger_count = logger::registry::instance().size();
    EXPECT_EQ(2, logger_count);
}

TEST(registry, get_stderr_sink) {
    logger::logger_ptr err = logger::get<logger::stderr_sink>("stderr-test-logger");

    EXPECT_NE(err, nullptr);
    EXPECT_EQ(err->name(), "stderr-test-logger");

    ::testing::internal::CaptureStderr();

    err->info("stderr sink test (a word: %s)", "hello, world");

    std::string output = ::testing::internal::GetCapturedStderr();

    auto pos = output.rfind("[L SUBSYS");
    EXPECT_EQ("[L SUBSYS=stderr-test-logger] stderr sink test (a word: hello, world)\n", output.substr(pos));
}

TEST(registry, get_stdout_sink) {
    logger::logger_ptr out = logger::get<logger::stdout_sink>("stdout-test-logger");

    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out->name(), "stdout-test-logger");

    ::testing::internal::CaptureStdout();
    out->info("stdout sink test (a word: %s)", "Hello, world");
    std::string output = ::testing::internal::GetCapturedStdout();

    auto pos = output.rfind("[L SUBSYS");
    EXPECT_EQ("[L SUBSYS=stdout-test-logger] stdout sink test (a word: Hello, world)\n", output.substr(pos));
}

TEST(registry, get_syslog_sink) {
    logger::logger_ptr user_log = logger::get<logger::syslog_sink>("syslog-test-logger");
    EXPECT_NE(user_log, nullptr);
    EXPECT_EQ(user_log->name(), "syslog-test-logger");
    user_log->info("sent by cpp-logger");

    logger::logger_ptr local1_log = logger::get<logger::syslog_sink>(
            "syslog-test-local1-logger",
            logger::syslog::local1_facility,
            0);

    EXPECT_NE(local1_log, nullptr);
    EXPECT_EQ(local1_log->name(), "syslog-test-local1-logger");
    local1_log->info("sent by cpp-logger");
}

TEST(logger, change_log_level) {
    logger::logger_ptr err = logger::get<logger::stdout_sink>("stderr-test-logger");
    EXPECT_NE(err, nullptr);
    EXPECT_EQ(err->name(), "stderr-test-logger");

    auto level = err->level();

    EXPECT_NE(level, logger::log_level::alert);

    logger::set_level(logger::log_level::alert);

    EXPECT_EQ(err->level(), logger::log_level::alert);
}

TEST(logger, change_ecid) {
    logger::logger_ptr out = logger::get<logger::stdout_sink>("stdout-test-logger");
    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out->name(), "stdout-test-logger");

    logger::set_ecid("NEW ECID");

    auto ecid = out->ecid();
    EXPECT_EQ(out->ecid(), "[M ECID=\"NEW ECID\"]");
}

TEST(logger, program_name) {

    logger::set_program_name("google-tests");
    logger::logger_ptr out = logger::get<logger::stdout_sink>("stdout-change-pname");

    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out->program_name(), "google-tests");

}

TEST(logger, stdout_logger) {

    std::string name { "logger-name"};
    logger::logger_ptr logger{
            new logger::logger{
                    name,
                    new logger::stdout_sink(name, "program", logger::log_level::info) // logger::logger is in charge of deleting the sink instance
            }
    };

    ASSERT_NE(logger, nullptr);

    ::testing::internal::CaptureStdout();
    logger->info("stdout sink test, name: %s", name.c_str());
    std::string output = ::testing::internal::GetCapturedStdout();
    auto pos = output.rfind("[L SUBSYS");

    EXPECT_EQ("[L SUBSYS=logger-name] stdout sink test, name: logger-name\n", output.substr(pos));
}
