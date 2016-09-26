/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include <unistd.h>

int main(){

  char hostname[100];
  gethostname(hostname, 100);

  pmu::log::set_program_name("logger-test");
  pmu::log::set_level(pmu::log::log_level::info);

  pmu::log::logger_ptr err    = pmu::log::get<pmu::log::stderr_sink>("stderr-test-logger");
  pmu::log::logger_ptr out    = pmu::log::get<pmu::log::stdout_sink>("stdout-test-logger");
  pmu::log::logger_ptr logger = pmu::log::get<pmu::log::stdout_sink>("stdout-test-logger");

  out->warning("registered %d loggers so far.\n", pmu::log::registry::instance().size());
  err->warning("writing into stderr sink");

  logger->info("starting logger test program on %s (using version: %s)\n", hostname, pmu::log::cpp_logger_version());

  logger->info("setting ECID to %s", "Test ecid !!!");
  logger->set_ecid("Test ecid !!!");

  logger->notice("setting log level to debug");

  logger->set_log_level(pmu::log::log_levels::debug);
  logger->debug("debug message");
  logger->info("info message");
  logger->warning ("warn message\n");

  logger->notice("setting log level to notice");

  logger->set_log_level(pmu::log::log_levels::notice);
  logger->debug("debug message");
  logger->info("info message");
  logger->warning ("warn message\n");

  logger->notice("setting log level to info");
  pmu::log::set_level(pmu::log::log_level::info);

  // for ( auto x = 1000000; x > 0 ; x--) {
  for ( auto x = 10; x > 0 ; x--) {
    logger->info("Messages #%d.", x);
  }

  logger->set_ecid("01234567801234567801234567801234567801234567801234567801234567899999990123456789");
  logger->info("very long ECID was set");

  logger->info("resetting ECID to empty");
  logger->set_ecid("");
  logger->info("done %s (%d)", __FILE__, __LINE__);
  logger->warning("End of test program. Have a nice day.");
}
