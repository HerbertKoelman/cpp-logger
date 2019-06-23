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

    logger::syslog_sink sink("syslog", "sink_tests", logger::log_level::info);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "syslog");
    sink.write(logger::log_level::warning,"write something to SysLog: %s", logger::cpp_logger_version());


    logger::set_program_name("sink_tests"); // default program name is app, change it to sink_tests
    logger::logger_ptr logger_1 = logger::get<logger::syslog_sink>("syslog", logger::syslog::user_facility, 0);
    logger_1->info("Hello, world ! (cpp-logger version %s)", logger::cpp_logger_version());
    logger::logger_ptr logger_2 = logger::get<logger::syslog_sink>("syslog");
}

TEST(sink, set_get_ecid) {

    logger::stdout_sink sink("stdout", "app", logger::log_level::info);

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "stdout");

    sink.set_ecid("a-new-ecid-value");
    EXPECT_EQ(sink.ecid(), "[M ECID=\"a-new-ecid-value\"]");

    sink.set_ecid(""); // reste ECID
    EXPECT_EQ(sink.ecid(), "- ");
}

TEST(sink, log_level_name) {
    class test_stdout_sink: public logger::stdout_sink{
    public:
        void test_log_level_name(){

            EXPECT_EQ(log_level_name(logger::log_level::emerg),   "EMERG");
            EXPECT_EQ(log_level_name(logger::log_level::alert),   "ALERT");
            EXPECT_EQ(log_level_name(logger::log_level::err),     "ERROR");
            EXPECT_EQ(log_level_name(logger::log_level::warning), "WARNING");
            EXPECT_EQ(log_level_name(logger::log_level::notice),  "NOTICE");
            EXPECT_EQ(log_level_name(logger::log_level::info),    "INFO");
            EXPECT_EQ(log_level_name(logger::log_level::debug),   "DEBUG");
            EXPECT_EQ(log_level_name(logger::log_level::trace),   "TRACE");
            EXPECT_EQ(log_level_name((logger::log_level) 100),    "UNKNOWN");

        }
    };

    test_stdout_sink sink;
    sink.test_log_level_name();
}


/** This a sample specialized sink.
 *
 */
class slog_sink: public logger::sink {
public:

    /** Setup QNX's system log access.
     *
     * @param opcode
     */
    explicit slog_sink(int opcode): logger::sink{"slog","app", logger::log_level::info}, _opcode{opcode}{
        // intentional
    }

    /** Send something to QNX's system log.
     *
     * @param level message's level
     * @param fmt a printf like format string.
     * @param ... format string parameters (like in printf)
     */
    void write(logger::log_level level, const char *fmt, ...) override {

        logger::log_level target_level =  this->level(); // level's accessor is threadsafe

        if (target_level >= level) {

            // fill a buffer with the user message
            va_list args1;
            va_list args2;

            va_start(args1, fmt); // initialize va_list
            va_copy(args2, args1); // save a copy args1 -> args2

            // this call only returns the actual number of bytes needed to store the message.
            // It doesn't count the needed end-of-string
            size_t buffer_size = vsnprintf(NULL, 0, fmt, args1) + 1; // plus 1 character to store \0
            va_end(args1); // don't need this one anymore

            // add one more character to handle \n (see code below)
            char buffer[buffer_size + 1];
            memset(buffer, 0, buffer_size + 1);

            // fill buffer with message ...
            vsnprintf(buffer, buffer_size, fmt, args2);
            size_t len = strlen(buffer);
            va_end(args2);

            // add a new line if not already there
            if (buffer[len - 1] != '\n') {
                buffer[len] = '\n'; // override ending \0 (that's why we provisioned for one more character above)
            }

            std::string ecid = this->ecid(); // get current value once.
            std::string pattern = "[L SUBSYS=" + name() + "] %s %s";

            //slogf(_opcode,
            //      level,
            //      _pattern.c_str(),
            //      ecid.empty() ? "" : ecid.c_str(),
            //      buffer);

            std::printf(
                       pattern.c_str(),
                       ecid.empty() ? "" : ecid.c_str(),
                       buffer);
        }

    }

private:
    int         _opcode;
};

TEST(sink, extend_sink) {

    slog_sink sink{0};

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "slog");

    logger::logger_ptr logger = logger::get<slog_sink>("slog_test", 0);
    logger->info( "Tada, you're done");

}
