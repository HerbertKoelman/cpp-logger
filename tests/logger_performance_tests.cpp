/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include <unistd.h>
#include <libgen.h>
#include <gtest/gtest.h>

const std::string PNAME{"performance"};

long long int run(const logger::logger_ptr logger, int loop){
    logger->set_ecid("01234567801234567801234567801234567801234567801234567801234567899999990123456789");
    std::string s("hello world...");

    auto start = std::chrono::high_resolution_clock::now();

    for (auto x = loop; x > 0; x--) {
        logger->info("Messages #%d. program: %s, some text %s, %s", x, "performance",
                          "01234567890123456789012345678901234567801234567801234567801234567801234567899999",
                          s.c_str());
        //printf("Messages #%d. program: %s, some text %s, %s", x, pname, "01234567890123456789012345678901234567801234567801234567801234567801234567899999", s.c_str());
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() ;

    logger->info("\n    Ran performance tests on version: %s. Wrote %d log entries in %d ms\n--------", logger::cpp_logger_version(), loop, duration);

    return duration;
}

TEST(logger_performance, default) {
    char hostname[100];
    gethostname(hostname, 100);

    logger::set_program_name(PNAME);
    logger::logger_ptr main_logger = logger::get("default");


    main_logger->info("starting logger test program on %s (using version: %s)", hostname, logger::cpp_logger_version());

    int loop = 10000;

    auto duration = run(main_logger, loop);

    std::cout << "called " << loop << " time logger->info(...) in " << duration << " milliseconds." << std::endl;

    EXPECT_LT(duration, 6000);
}

TEST(logger_performance, stdout_sink) {
    char hostname[100];
    gethostname(hostname, 100);

    logger::set_program_name(PNAME);
    logger::logger_ptr stdout_logger = logger::get<logger::stdout_sink>("stdout");


    stdout_logger->info("starting logger test program on %s (using version: %s)", hostname, logger::cpp_logger_version());

    int loop = 10000;

    auto duration = run(stdout_logger, loop);

    std::cout << "called " << loop << " time logger->info(...) in " << duration << " milliseconds." << std::endl;

    EXPECT_LT(duration, 6000);
}

TEST(logger_performance, stderr_sink) {
    char hostname[100];
    gethostname(hostname, 100);

    logger::set_program_name(PNAME);
    logger::logger_ptr stdout_logger = logger::get<logger::stderr_sink>("stderr");


    stdout_logger->info("starting logger test program on %s (using version: %s)", hostname, logger::cpp_logger_version());

    int loop = 10000;

    auto duration = run(stdout_logger, loop);

    std::cout << "called " << loop << " time logger->info(...) in " << duration << " milliseconds." << std::endl;

    EXPECT_LT(duration, 6000);
}

TEST(logger_performance, syslog_sink) {
    char hostname[100];
    gethostname(hostname, 100);

    logger::set_program_name(PNAME);
    logger::logger_ptr stdout_logger = logger::get<logger::syslog_sink>("syslog");


    stdout_logger->info("starting logger test program on %s (using version: %s)", hostname, logger::cpp_logger_version());

    int loop = 10;

    auto duration = run(stdout_logger, loop);

    std::cout << "called " << loop << " time logger->info(...) in " << duration << " milliseconds." << std::endl;

    EXPECT_LT(duration, 5);
}

/*
int main( int argc, char *argv[]){

  char hostname[100];
  gethostname(hostname, 100);
  char *pname = basename(argv[0]);

  logger::set_program_name(pname);
  logger::logger_ptr main_logger = logger::get("main");

  main_logger->info("starting logger test program on %s (using version: %s)", hostname, logger::cpp_logger_version());

  if ( argc == 1 ){
    main_logger->set_ecid("01234567801234567801234567801234567801234567801234567801234567899999990123456789");
    std::string s("hello world...");
    int x =100;

    while (true) {
      main_logger->info("Messages #%d. program: %s, some text %s, %s", x, pname, "01234567890123456789012345678901234567801234567801234567801234567801234567899999", s.c_str());
      //printf("Messages #%d. program: %s, some text %s, %s", x, pname, "01234567890123456789012345678901234567801234567801234567801234567801234567899999", s.c_str());
    }
    main_logger->set_ecid("");
  } else if ( argc == 2 ){
    int loop = atoi(argv[1]);

    main_logger->set_ecid("01234567801234567801234567801234567801234567801234567801234567899999990123456789");
    std::string s("hello world...");
    int x =100;

    for ( auto x = loop; x > 0 ; x--) {
      main_logger->info("Messages #%d. program: %s, some text %s, %s", x, pname, "01234567890123456789012345678901234567801234567801234567801234567801234567899999", s.c_str());
      //printf("Messages #%d. program: %s, some text %s, %s", x, pname, "01234567890123456789012345678901234567801234567801234567801234567801234567899999", s.c_str());
    }
    main_logger->set_ecid("");
  } else {
    main_logger->err("usage: %s [counter]" , pname );
  }

  main_logger->warning("end of test program %s.", pname);
}
*/