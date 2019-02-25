/** simple test program.
 *
 * It's used to check that the library is working as expected.
 */
#include <logger/cpp-logger.hpp>
#include <unistd.h>
#include <libgen.h>

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
