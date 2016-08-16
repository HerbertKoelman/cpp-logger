/*
 * pmu::logger - herbert koelman
 *
 * this files gathers all definition and defines. it helps to avoid include recursion issues.
 *
 */

#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread/pthread.hpp>

#ifndef PMU_LOG_DEFINITIONS_HPP
#define PMU_LOG_DEFINITIONS_HPP

#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7
#define LOG_TRACE   8

#define MAXECIDLEN  64
// original #define PMU_LOG_PATTERN "<%d> %s %s [%s] (pid: %d, thrdid: %d, ecid: %s) - "
#define PMU_LOG_PATTERN "<%d> %s %s %s %s.%.6d.%.6d - %-16s "

namespace pmu {
  namespace log {

   /** \addtogroup pmu_log_definitions Type definitions and macros
    *  \addtogroup pmu_log
    * @{
    */

    class logger;

    /** known logging levels */
    enum log_levels {
      emerg   = LOG_EMERG, //!< system is unusable
      alert   = LOG_ALERT, //!< action must be taken immediately
      crit    = LOG_CRIT,  //!< critical conditions
      err     = LOG_ERR,   //!< error conditions
      warning = LOG_WARNING, //!< warning condition
      notice  = LOG_NOTICE,  //!< normal but signification condition
      info    = LOG_INFO,  //!< informational (default)
      debug   = LOG_DEBUG, //!< debug information
      trace   = LOG_TRACE  //!< trace information
    };

    /** used when defined as a parameter */
    typedef log_levels log_level;

    /** known facilities */
    enum log_facilities {
      sic_bat, //!< pour les batchs
      sic_ine, //!< pour les interfaces externes
      sic_bmp, //!< Pour les BMP IMS
      sic_mpp, //!< Pour le MPP IMS
      sic_kix, //!< Pour le CICS
      sic_tux  //!< (non encore utilis.)
    };

    /** used to define method paramters */
    typedef log_facilities log_facility;

#if __IBMCPP_TR1__ //NOSONAR this macro is set with the compiler command line argument
    typedef std::tr1::shared_ptr<pmu::log::logger> logger_ptr; //!< shared pointer to a logger instance
#else
    typedef std::shared_ptr<pmu::log::logger>      logger_ptr; //!< shared pointer to a logger instance
#endif

    /** @} */

  } // namespace log
} // namespace pmu
#endif
