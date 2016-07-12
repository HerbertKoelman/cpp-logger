#include <logger/logger.hpp>
#include <logger/registry.hpp>

#ifndef PMU_CPP_LOGGER_HPP
#define PMU_CPP_LOGGER_HPP

//! \namespace pmu 
namespace pmu {
  //! \namespace log 
  namespace log {

   /** \addtogroup pmu_log pmu logging facility for C++.
    * @{
    */
  
   /** @return the library's current version
    */
    extern "C" const char *cpp_logger_version();

   /** @} */
  }
}
#endif
