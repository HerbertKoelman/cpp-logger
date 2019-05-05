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

TEST(registry, get_stderr_sink) {
    logger::logger_ptr err = logger::get<logger::stderr_sink>("stderr-test-logger");

    EXPECT_NE(err, nullptr);
    EXPECT_EQ(err->name(), "stderr-test-logger");

    ::testing::internal::CaptureStderr();

    err->info("stderr sink test (version: %s)", logger::cpp_logger_version());

    std::string output = ::testing::internal::GetCapturedStderr();

    EXPECT_TRUE( output.rfind("[L SUBSYS=stderr-test-logger] stderr sink test")!= std::string::npos);
}

TEST(registry, get_stdout_sink) {
    logger::logger_ptr out = logger::get<logger::stdout_sink>("stdout-test-logger");

    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out->name(), "stdout-test-logger");

    ::testing::internal::CaptureStdout();
    out->info("stdout sink test (version: %s)", logger::cpp_logger_version());
    std::string output = ::testing::internal::GetCapturedStdout();

    EXPECT_TRUE( output.rfind("[L SUBSYS=stdout-test-logger] stdout sink test")!= std::string::npos);
}

TEST(registry, get_syslog_sink) {
    logger::logger_ptr user_log = logger::get<logger::syslog_sink>("syslog-test-logger");
    EXPECT_NE(user_log, nullptr);
    EXPECT_EQ(user_log->name(), "syslog-test-logger");
    user_log->info("sent by cpp-logger");

    logger::logger_ptr local1_log = logger::get<logger::syslog_sink>(
            "syslog-test-local1-logger",
            "local1",
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

TEST(logger, DISABLED_legacy) {
    char hostname[100];
    gethostname(hostname, 100);

    logger::set_program_name("logger-test");
    logger::set_level(logger::log_level::info);

    logger::logger_ptr err = logger::get<logger::stderr_sink>("stderr-test-logger");
    logger::logger_ptr out = logger::get<logger::stdout_sink>("stdout-test-logger");
    logger::logger_ptr logger = logger::get<logger::stdout_sink>("stdout-test-logger");

    out->info("Running version: %s" , logger::cpp_logger_version());
    out->info("get a syslog logger [syslog-test-logger]");
    out->warning("check your syslogd config to locate the destination file");
    logger::logger_ptr syslog = logger::get<logger::syslog_sink>("syslog-test-logger");
    out->info("sending messages to [syslog-test-logger]");
    syslog->info("syslog logger says hello %s", "herbert");
    syslog->crit("syslog logger says hello %s", "herbert");
    syslog->trace("TRACE syslog logger says hello %s", "herbert");

    logger::logger_ptr test_logger = logger::get("test-logger");

    out->warning("registered %d loggers so far.\n", logger::registry::instance().size());
    err->warning("writing into stderr sink");

    logger->info("starting logger test program on %s (using version: %s)\n", hostname, logger::cpp_logger_version());

    logger->info("setting ECID to %s", "Test ecid !!!");
    logger->set_ecid("Test ecid !!!");

    logger->notice("setting log level to debug");
    std::string str("hello, world...");
    logger->notice("string [%s], decimal: %d, long decimal: %ld.\n\n", str.c_str(), 10, 100000);
    logger->notice("End of line handling, string [%s], decimal: %d, long decimal: %ld.", str.c_str(), 10, 100000);

    logger->set_ecid("");
    logger->notice("string [%s], decimal: %d, long decimal: %ld.\n\n", str.c_str(), 10, 100000);
    logger->notice("End of line handling, string [%s], decimal: %d, long decimal: %ld.", str.c_str(), 10, 100000);

    logger->set_log_level(logger::log_levels::debug);
    logger->debug("debug message");
    logger->info("info message");
    logger->warning("warn message\n");

    logger->notice("setting log level to notice");

    logger->set_log_level(logger::log_levels::notice);
    logger->debug("debug message");
    logger->info("info message");
    logger->warning("warn message\n");

    logger->notice("setting log level to info");
    logger::set_level(logger::log_level::info);

    for (auto x = 10; x > 0; x--) {
        test_logger->info("Messages #%d.\n", x);
    }

    logger->set_ecid("01234567801234567801234567801234567801234567801234567801234567899999990123456789");
    logger->info("very long ECID was set");

    logger->info("resetting ECID to empty");
    logger->set_ecid("");
    logger->info("done %s (%d)", __FILE__, __LINE__);
    logger->warning("End of test program Have a nice day.");
}
