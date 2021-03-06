/* created : Thu Jul  7 18:38:27 CEST 2016
 * author: herbert koelman (herbert.koelman@me.com)
 */

#include "logger/facilities.hpp"

#if __cplusplus >= 201703L
#include <shared_mutex>
#else
#include <mutex>
#endif

#include <thread> // std::mutex
#include <atomic>
#include <cstdio>   // std::vsnprintf(...)
#include <cstdarg>  // std::va_list, ...
#include <string>   // std::string
#include <vector>   // std::unordered_map
#include <unistd.h> // std::getpid
#include <limits>

#ifndef CPP_LOGGER_SINKS_HPP
#define CPP_LOGGER_SINKS_HPP

#include "logger/definitions.hpp"
#include <logger/facilities.hpp>
#include <logger/exceptions.hpp>

namespace logger {
    /** \addtogroup logger_log
     * @{
     */

    /** Logging message sink (destination).
     *
     * A sink is in charge of sending messages to a specific destination in a proper way. Loggers delegate to instances of this
     * class the actual writing, sending or whatever it takes to handle a log entry.
     *
     * Specialize this class to create your own logger sinks.
     *
     * @author herbert koelman
     * @since v1.4.0
     */
    class sink {
    public:

        friend class registry; //!< this will let registry's factory setup sinks in a simplified way

        /** write operation.
         *
         * A sink should override this virtual pure method in order to provide the write method to logger instances.
         *
         * @param level corresponding messages's log level.
         * @param fmt formatting string
         * @param ... format parameters
         */
        virtual void write(log_level level, const char *fmt, ...) = 0;

        /** change the current log level.
         *
         * @param level new logging level
         */
        void set_log_level(log_levels level);

        /** @return niveau courrant de journalisation
         */
        log_levels level() ;

        /** @return logger name */
        std::string &name() {
            return _name;
        };

        /** @return program name */
        std::string &program_name() {
            return _pname ;
        }

        /** change the current ecid.
         *
         * Setting this to an empty string will stop logger to prin
         *
         * @param ecid new ecid
         */
        void set_ecid(const std::string &ecid);

        /** @return current execution ID.
         */
        std::string ecid();

        /** dispose of logger instance ressources
         */
        virtual ~sink();

    protected:

        /** new class instance.
         *
         * > **WARN** these parameters are automatically passed when using the registry facility.
         *
         * @param name sink's name
         * @param pname program name
         * @param level initial log level (defaults to logger::log_level::info)
         */
        explicit sink(const std::string &name, const std::string &pname = "prog", log_level level = log_levels::info);

        /** @return display name for a given log level
         */
        virtual std::string log_level_name(log_level level);

        /** set program name.
         *
         * @param name program name.
         */
         virtual void set_program_name(const std::string &name);

         /** set sink's subsystem name.
          *
          * @param name subset name.
          */
         virtual void set_name (const std::string &name);

    private:
#if __cplusplus >= 201703L
        std::shared_mutex _shared_mutex; //!< used to protect access to sink's data
#else
        std::mutex        _mutex;        //!< used to protect access to sink's data
#endif

        // when _ecid is accessed, we need to do more than just set/get it's value. Therefore, we cannot use std::atomic
        std::string       _ecid;   //!< execution control ID. Helps to track everything that was logged by one business operation

        // these are read-only, we don't need to handle concurrency
        std::string       _name;   //!< logging domain name (as for now, this is equal to the logger name)
        std::string       _pname;  //!< program name

        std::atomic<log_level>     _level;  //!< current logging level

    }; // sink

    /** file sink.
     *
     * send log messages to FILE.
     *
     * > **WARNING** it is up to you to handle the file opening/closing.
     *
     * @author herbert koelman
     * @since v1.4.0
     */
    class file_sink : public sink {
    public:

        /** new instance.
         *
         * @param name sink name
         * @param pname program name
         * @param file output file.
         * @param level initial log level (defaults to logger::log_levels::info)
         */
        file_sink(const std::string &name, const std::string &pname, log_level level, FILE *file);

        /** new instance.
         *
         * Default initial values:
         * - name: "file-sink",
         * - program_name: "app",
         * - level: log_level::info
         *
         * @param file output file.
         */
        explicit file_sink(FILE *file);

        /** \copydoc sink::write()
         *
         * This sink writes messages in FILE.
         *
         */
        void write(log_level level, const char *fmt, ...) override ;

    protected:

        /** Set sybsystem name and reset fixed part of the output pattern.
         *
         * @param name sink name
         */
        void set_name(const std::string &name) override ;

        /** fill the buffer with the current date and time information
         */
        const std::string date_time();

    private:

        FILE             *_file_descriptor; //!< file descriptor of a log file
        pid_t             _pid;      //!< process ID
        std::string       _lag;      //!< date time lag (i.e. +02:00)
        std::string       _hostname; //!< hostname (this will be displayed by log messages)
        std::string       _pattern;  //!< message pattern (layout)
    };

    /** stdout sink.
     *
     * send log messages to stdout
     *
     * @author herbert koelman
     * @since v1.4.0
     */
    class stdout_sink : public file_sink {
    public:

        /** new instance.
         *
         * @param name sink name
         * @param pname program name
         * @param level initial log level (defaults to logger::log_levels::info)
         */
        stdout_sink(const std::string &name, const std::string &pname, log_level level);

        /** new instance.
         */
        explicit stdout_sink() ;

    };

    /** stderr sink.
     *
     * send log messages to stderr
     *
     * @author herbert koelman
     * @since v1.4.0
     */
    class stderr_sink : public file_sink {
    public:

       /** new instance.
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to logger::log_levels::info)
         */
        stderr_sink(const std::string &name, const std::string &pname, log_level level);

        /** new instance.
         */
        explicit stderr_sink();

    };

    /** syslog sink.
     *
     * Send log messages to syslogd (more [here(https://en.wikipedia.org/wiki/Syslog))
     *
     * > **WARN**  The syslog_sink, considers that TRACE and DEBUG are the same.
     *
     * @author herbert koelman
     * @since v1.4.0
     */
    class syslog_sink : public sink {
    public:

        /** new instance.
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to logger::log_levels::info)
         * @param facility syslog facility to use (default is "user")
         * @param options syslog options, the argument is an OR of any of these LOG_CONS, LOG_NDELAY, LOG_NOWAIT, LOG_ODELAY, LOG_PID (this is the default)
         * @throws sink_exception if init went wrong
         */
        syslog_sink(const std::string &name, const std::string &pname, log_level level, const syslog::facility &facility, int options );

       /** new instance.
         *
         * @param facility syslog facility to use (default is "user")
         * @param options syslog options
         * @throws sink_exception if init went wrong
         * @see syslog::user_facility
         */
        syslog_sink(const syslog::facility &facility, int options);

       /** new instance.
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to logger::log_levels::info)
         */
        syslog_sink(const std::string &name, const std::string &pname, log_level level);

       /** new instance.
         *
         */
        explicit syslog_sink();

        ~syslog_sink() = default ;

        /** \copydoc  sink::write
         *
         * send messages to syslogd
         */
        void write(log_level level, const char *fmt, ...) override ;

    protected:

        void set_name(const std::string &name) override ;

    private:
        std::string _pattern; //!< message pattern (layout)
    };

    /** @} */

} // namespace logger
#endif
