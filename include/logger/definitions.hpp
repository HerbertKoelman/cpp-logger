/*
 * logger - herbert koelman
 *
 * this files gathers all definition and defines. it helps to avoid include recursion issues.
 *
 */

#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#ifndef CPP_LOG_DEFINITIONS_HPP
#define CPP_LOG_DEFINITIONS_HPP

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
#define LOGGER_LOG_PATTERN "<%d>1 %s %s %s %s.%d.%d - %-16s"

namespace logger {

    /** \addtogroup logger_definitions Type definitions and macros
     * @{
     */


    const static long HOST_NAME_MAX = sysconf(_SC_HOST_NAME_MAX);
    //if (host_name_max <= 0) host_name_max = _POSIX_HOST_NAME_MAX;

    class logger;

    /** known logging levels */
    enum log_levels {
        emerg = LOG_EMERG, //!< system is unusable
        alert = LOG_ALERT, //!< action must be taken immediately
        crit  = LOG_CRIT,  //!< critical conditions
        err   = LOG_ERR,   //!< error conditions
        warning = LOG_WARNING, //!< warning condition
        notice  = LOG_NOTICE,  //!< normal but signification condition
        info  = LOG_INFO,  //!< informational (default)
        debug = LOG_DEBUG, //!< debug information
        trace = LOG_TRACE  //!< trace information
    };

    /** used to define method paramters (readability) */
    typedef log_levels log_level;

    enum log_facilities {
        sic_bat, //!< pour les batchs
        sic_ine, //!< pour les interfaces externes
        sic_bmp, //!< Pour les BMP IMS
        sic_mpp, //!< Pour le MPP IMS
        sic_kix, //!< Pour le CICS
        sic_tux  //!< (non encore utilis.)
    };

    /** used to define method paramters readability) */
    //typedef log_facilities log_facility;

#if __IBMCPP_TR1__ //NOSONAR this macro is set with the compiler command line argumen
    typedef std::tr1::shared_ptr<logger::logger> logger_ptr; //!< shared pointer to a logger instance
#else
    typedef std::shared_ptr<logger> logger_ptr; //!< shared pointer to a logger instance
#endif

    /** @} */

} // namespace logger
#endif
