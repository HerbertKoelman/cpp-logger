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
#include "facilities.hpp"

#ifndef CPP_LOG_DEFINITIONS_HPP
#define CPP_LOG_DEFINITIONS_HPP

// #define LOGGER_LOG_PATTERN "<%d>1 %s %s %s.%d.%d - %-16s"

namespace logger {

    /** \addtogroup logger_definitions Type definitions and macros
     * @{
     */

    /* The constexpr specifier declares that it is possible to evaluate the value of the function or variable at compile time.
     */
    constexpr short LOG_EMERG    = 0;
    constexpr short LOG_ALERT    = 1;
    constexpr short LOG_CRIT     = 2;
    constexpr short LOG_ERR      = 3;
    constexpr short LOG_WARNING  = 4;
    constexpr short LOG_NOTICE   = 5;
    constexpr short LOG_INFO     = 6;
    constexpr short LOG_DEBUG    = 7;
    constexpr short LOG_TRACE    = 8;
    constexpr short MAXECIDLEN   = 64;
    constexpr char const *LOGGER_LOG_PATTERN = "<%d>1 %s %s %s.%d.%d - %-16s";

    const long HOST_NAME_MAX = sysconf(_SC_HOST_NAME_MAX);
    //if (host_name_max <= 0) host_name_max = _POSIX_HOST_NAME_MAX;

    class logger;

    /** known logging levels */
    enum log_levels {
        emerg    = LOG_EMERG, //!< system is unusable
        alert    = LOG_ALERT, //!< action must be taken immediately
        crit     = LOG_CRIT,  //!< critical conditions
        err      = LOG_ERR,   //!< error conditions
        warning  = LOG_WARNING, //!< warning condition
        notice   = LOG_NOTICE,  //!< normal but signification condition
        info     = LOG_INFO,  //!< informational (default)
        debug    = LOG_DEBUG, //!< debug information
        trace    = LOG_TRACE  //!< trace information (this is not part the standard)
    };

    //! Classes and definitions only related to syslog logging.
    namespace syslog {

        /** SysLog standard facility's codes */
        enum facility_code{
            kern = 0, //!<Kernel messages
            user = 1, //!<User-level messages
            mail = 2, //!< Mail system
            daemon = 3, //!<System daemons
            auth   = 4, //!< Security/authentication messages
            syslog = 5, //!< Messages generated internally by syslogd
            lpr	   = 6, //!< Line printer subsystem
            news   = 7, //!<Network news subsystem
            uucp   = 8, //!< UUCP subsystem
            cron   = 9, //!< Clock daemon
            authpriv = 10, //!< Security/authentication messages
            ftp    = 11, //!< FTP daemon
            ntp    = 12, //!< NTP subsystem
            security = 13, //!< Log audit
            console  = 14, //!< Log alert
            solaris_cron = 15, //!< Scheduling daemon
            local0 = 16, //!< Local 0
            local1 = 17 //!< Local 1
        };
    }

    /** used to define method paramters (readability) */
    typedef log_levels log_level;

#if __IBMCPP_TR1__ //NOSONAR this macro is set with the compiler command line argumen
    typedef std::tr1::shared_ptr<logger::logger> logger_ptr; //!< shared pointer to a logger instance
#else
    typedef std::shared_ptr<logger> logger_ptr; //!< shared pointer to a logger instance
#endif

    /** @} */

} // namespace logger
#endif
