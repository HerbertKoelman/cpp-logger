/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>

void get_test( const std::string &name, pmu::log::log_level level){

  std::cout << "created function not a template. " << name << std::endl ;
}

template<class T, typename... Args> void get_test( const std::string &name, pmu::log::log_level level, const Args&... args){

  T instance(name);
  std::cout << "created string: " << instance << std::endl ;
}

class C {


  public:

    C(): _log(pmu::log::get("test-class-c")){
      _log->info("new class instance");
    };

  private:
    pmu::log::logger_ptr _log;
};

class D : public C{
  public:

    D(): _log(pmu::log::get("test-class-d")){
      _log->info("new class instance");
    };

  private:
    pmu::log::logger_ptr _log;
};

int main(){

  get_test("hello les amis du C++", pmu::log::log_level::info);

  get_test<std::string>("hello", pmu::log::log_level::info,  "one", 2);

  C cclass;
  D dclass;

  char hostname[100];
  gethostname(hostname, 100);

  pmu::log::set_program_name("logger-test");

  pmu::log::set_level(pmu::log::log_level::warning);
  pmu::log::logger_ptr logger( new pmu::log::logger());

  logger->info("starting logger test program on %s (using version: %s)", hostname, pmu::log::cpp_logger_version());

  logger->info("try to get regsitry singleton");
  pmu::log::registry reg = pmu::log::registry::instance();

  logger->info("About to set ECID to %s", "Test ecid !!!");
  logger->set_ecid("Test ecid !!!");

  logger->info("try to get a new logger instance through the template get method");

  pmu::log::logger_ptr template_logger = pmu::log::get<pmu::log::logger>("template-logger");
  template_logger->crit("HOURRAAAA ca marche >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

  logger->set_log_level(pmu::log::log_levels::debug);
  logger->debug("got one");

  logger->info("Hello, %s", "world" );

  logger->warning ("level: %d\n", reg.level());

  pmu::log::logger_ptr flogger1 = pmu::log::get("test-logger");
  pmu::log::logger_ptr flogger2 = pmu::log::get("test-logger");
  flogger1->info("factory created logger one name: %s, level: %d", flogger1->name().c_str(), flogger1->level());
  flogger1->set_log_level(pmu::log::log_levels::warning);
  flogger2->info("factory created logger one name: %s, level: %d", flogger2->name().c_str(), flogger2->level());

  pmu::log::set_level(pmu::log::log_level::warning);
  pmu::log::logger_ptr main_logger = pmu::log::get("main");

  // for ( auto x = 1000000; x > 0 ; x--) {
  for ( auto x = 10; x > 0 ; x--) {
    main_logger->info("Messages #%d.", x);
  }

  logger->set_ecid("01234567801234567801234567801234567801234567801234567801234567899999990123456789");
  logger->info("very long ECID was set");

  logger->info("resetting ECID to empty");
  logger->set_ecid("");
  logger->info("done %s (%d)", __FILE__, __LINE__);

  main_logger->warning("End of test program. Have a nice day.");
}
