//
//  cpp_pthread.hpp
//  cpp_pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef cpp_logger_hpp
#define cpp_logger_hpp

#include "config.h"
#include "logger/cpp-logger.hpp"

namespace logger {

    extern "C" const char *cpp_logger_version(){

    #ifndef CPP_LOGGER_VERSION
      return "missing CPP_LOGGER_VERSION define. Re-run configure" ;
    #else
      return CPP_LOGGER_VERSION ;
    #endif
  }
}
#endif /* cpp_logger_hpp */
