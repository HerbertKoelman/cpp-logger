#include <logger/logger.hpp>
#include <logger/registry.hpp>
#include <logger/sinks.hpp>

#ifndef CPP_CPP_LOGGER_HPP
#define CPP_CPP_LOGGER_HPP


//! \namespace log
namespace logger {

 /** \addtogroup logger_log pmu logging facility for C++.
  * @{
  */

 /** @return the library's current version
  */
  extern "C" const char *cpp_logger_version();

 /** @} */
  }

#endif
