/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include "gtest/gtest.h"
#include <ctime>
#include <sys/time.h>

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

        char hostname[logger::HOST_NAME_MAX];
        hostname[0] = 0;
        gethostname(hostname, logger::HOST_NAME_MAX);
        _hostname = hostname;

        timeval current_time{0};
        gettimeofday(&current_time, nullptr);

        struct std::tm local_time{0};
        localtime_r(&current_time.tv_sec, &local_time);

        char buffer[10];// lag is in the form of "-02:00"
        memset(buffer, 0, 10);

        int lag = (timezone / 3600) * (-1 * ((local_time.tm_isdst == 1) ? 200 : 100));
        int hours = lag / 100;
        int minutes = lag - (hours * 100);
        snprintf(buffer, 7, "%s%0.2d:%0.2d",
                 ((lag < 0) ? "-" : "+"),
                 hours,
                 minutes
        );
        _lag = buffer;
    }

    template<typename... Args> void write (logger::log_level level, const std::string &fmt, const Args&... args){


        std::cout << "Hello" << std::endl;

        std::string ecid = this->ecid(); // get current value once.
        std::string pattern = "<%d> %s %s : [L SUBSYS=" + name() + "] %s " + fmt ;

        std::printf(
                pattern.c_str(),
                level,
                date_time().c_str(),
                _hostname.c_str(),
                ecid.empty() ? "" : ecid.c_str(),
                args...);
    }

    const std::string date_time() {
        int size = 50;
        char buffer[size];
        char target[size];

        memset(buffer, 0, size);
        memset(target, 0, size);

        timeval current_time{0};
        gettimeofday(&current_time, nullptr);
        int micros = current_time.tv_usec;

        struct std::tm local_time{0};
        localtime_r(&current_time.tv_sec, &local_time);
        snprintf(target, size - 1, "%d-%02d-%02dT%02d:%02d:%02d.%06d%s",
                 local_time.tm_year + 1900, // tm_year is the number of years from 1900
                 local_time.tm_mon + 1,   // tm_mon is the month number starting from 0
                 local_time.tm_mday,
                 local_time.tm_hour,
                 local_time.tm_min,
                 local_time.tm_sec,
                 micros,
                 _lag.c_str());

        return std::string(target);
    }


    void write(logger::log_level level, const char *fmt, ...) override {
        std::printf("We should pass here !!!\n");
    }

private:
    int         _opcode;
    std::string _lag;
    std::string _hostname;
};

TEST(sink, extend_sink) {

    slog_sink sink{0};

    EXPECT_EQ(sink.level(), logger::log_level::info);
    EXPECT_EQ(sink.name(), "slog");

    sink.write(logger::log_level::warning, (std::string)"TEMPLATE -->> %s: %d, %s: %ld.\n", "number", 10, "long number", 10000L);

    logger::logger_ptr logger = logger::get<slog_sink>("slog_test", 0);
    logger->info( "Tada, you're done");

}
