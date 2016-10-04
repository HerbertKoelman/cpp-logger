/* created : Thu Jul  7 18:38:27 CEST 2016
 * author: herbert koelman (herbert.koelman@pmu.fr)
 */

#include <pthread/pthread.hpp> // pthread::this_thread::get_id
#include <string>
#include <cstdio>   // std::vsnprintf(...)
#include <cstdarg>  // std::va_list, ...
#include <vector>
#include <unistd.h> // std::getpid

#ifndef PMU_LOGGER_SINKS_HPP
#define PMU_LOGGER_SINKS_HPP

#include "logger/definitions.hpp"

#define MAXECIDLEN 64

namespace pmu {
  namespace log {
   /** \addtogroup pmu_log
    * @{
    */

    /** abstract sink base class.
     *
     * derive specific implmenetation from this abstraction.
     *
     * @author herbert koelman
     * @since v1.4.0
     */
    class sink {
      public:

        /** actual write operation.
         *
         * @param level corresponding messages's log level.
         * @param fmt formatting string
         * @param ... format parameters
         */
        virtual void write( log_level level, const char *fmt, ... ) = 0;

        /** dispose of logger instance ressources
         */
        virtual ~sink();

        /** change the current log level.
         *
         * @param level new logging level
         */
        void set_log_level( log_levels level ){
          _level = level;
        };

        /** @return niveau courrant de journalisation
         */
        log_levels level() const {
           return _level;
        };

        /** @return logger name */
        std::string name() const{
          return _name;
        };

        /** @return logger's facility (see pmu::log::log_facility) */
        const std::string facility() const {
          return _facility;
        };

        /** set faclity name
         *
         * @param facility facility name
         */
        void set_facility(log_facility facility);

				/** change the current ecid.
				 *
				 * Setting this to an empty string will stop logger to prin
				 *
				 * @param ecid new ecid
				 */
				void set_ecid( const std::string &ecid );

				/** @return ecid courran
				 */
				std::string ecid() ;

      protected:

        /** instancie un objet pour journaliser
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to pmu::log::info)
         */
        sink( const std::string &name = "default", const std::string &pname = "prog", log_level level = log_levels::info );

        /** fill the buffer with the current date and time information
         */
        const std::string date_time();

        std::string     _name ; //!< logging domain name (as for now, this is equal to the logger name)
        std::string     _pattern;//!< message pattern (layout)
        std::string     _pname ; //!< program name
        std::string     _facility; //!< current faility string
        std::string     _ecid;     //!< current ECID (a Tuxedo notion)
        log_level       _level;    //!< current logging level
        pid_t           _pid;      //!< process ID
        std::string     _lag;      //!< date time lag (i.e. +02:00)
        char            _hostname[HOST_NAME_MAX]; //!< hostname (this will be displayed by log messages)

      private:
        pthread::read_write_lock _ecid_rwlock; //!< ecid access protection
        pthread::mutex           _mutex;       //!< used to protect access to static class data

    }; // sink

    /** file sink.
     *
     * send log messages to FILE.
     *
     * > it is up to you to handle the file opening/closing.
     *
     * @author herbert koelman (herbert.koelman@pmu.fr)
     * @since v1.4.0
     */
    class file_sink: public sink {
      public:

        /** instancie un objet pour journaliser
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to pmu::log::info)
         */
        file_sink( const std::string &name, const std::string &pname, log_level level, FILE *file);

        virtual ~file_sink();

        /** \copydoc sink::write()
         *
         * This sink writes messages in FILE.
         *
         */
        virtual void write( log_level level, const char *fmt, ... );

      protected:
        FILE   *_file_descriptor ; //!< file descriptor of a log file
    };

    /** stdout sink.
     *
     * send log messages to stdout
     *
     * @author herbert koelman (herbert.koelman@pmu.fr)
     * @since v1.4.0
     */
    class stdout_sink: public file_sink {
      public:

        /** instancie un objet pour journaliser
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to pmu::log::info)
         */
        stdout_sink( const std::string &name = "stdout", const std::string &pname = "prog", log_level level = log_level::info);

    };

    /** stderr sink.
     *
     * send log messages to stderr
     *
     * @author herbert koelman (herbert.koelman@pmu.fr)
     * @since v1.4.0
     */
    class stderr_sink: public file_sink {
      public:

        /** instancie un objet pour journaliser
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to pmu::log::info)
         */
        stderr_sink( const std::string &name = "stderr", const std::string &pname = "prog", log_level level = log_level::info);

    };

    /** syslog sink.
     *
     * send log messages to syslogd
     *
     * @author herbert koelman (herbert.koelman@pmu.fr)
     * @since v1.4.0
     */
    class syslog_sink: public sink {
      public:

        /** instancie un objet pour journaliser
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to pmu::log::info)
         * @param facility syslog facilty to use (default is LOG_USER)
         * @param options syslog options
         */
        syslog_sink( const std::string &name = "syslog", const std::string &pname = "prog", log_level level = log_level::info, int facility = NULL, int options = NULL);

        virtual ~syslog_sink();

        /** \copydoc  sink::write
         *
         * send messages to syslogd
         */
        virtual void write( log_level level, const char *fmt, ... );
    };

    /** @} */

  } // namespace log
} // namespace pmu
#endif
