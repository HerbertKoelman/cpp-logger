#include <logger.hpp>
#include <registry.hpp>

int main(){

  pmu::log::logger_ptr logger( new pmu::log::logger());

  logger->info("starting test program");

  logger->info("try to get regsitry singleton");
  pmu::log::registry reg = pmu::log::registry::instance();
  logger->set_log_level(pmu::log::log_levels::debug);
  logger->debug("got one");

  logger->info("Hello, %s", "world" );

  logger->warning ("level: %d\n", reg.level());

  pmu::log::logger_ptr flogger1 = pmu::log::get("test-logger");
  pmu::log::logger_ptr flogger2 = pmu::log::get("test-logger");
  flogger1->info("factory created logger one name: %s, level: %d", flogger1->name().c_str(), flogger1->level());
  flogger1->set_log_level(pmu::log::log_levels::warning);
  flogger2->info("factory created logger one name: %s, level: %d", flogger2->name().c_str(), flogger2->level());

  pmu::log::logger_ptr main_logger = pmu::log::get("main");

  for ( auto x = 1000000; x > 0 ; x--) {
  // for ( auto x = 10; x > 0 ; x--) {
    main_logger->info("Messages #%d.", x);
  }

  main_logger->warning("End of test program. Have a nice day.");
}
