/* created : Thu Jul  7 18:38:27 CEST 2016
 * author: herbert koelman (herbert.koelman@me.com)
 */

#include "logger/facilities.hpp"

#include <mutex>
#include <shared_mutex>
#include <thread>

#include <string>
#include <cstdio>   // std::vsnprintf(...)
#include <cstdarg>  // std::va_list, ...
#include <vector>
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

        /** write operation.
         *
         * A sink should override this virtual pure method in order to provide the write method to logger instances.
         *
         * @param level corresponding messages's log level.
         * @param fmt formatting string
         * @param ... format parameters
         */
        virtual void write(log_level level, const char *fmt, ...) = 0;

        /** dispose of logger instance ressources
         */
        virtual ~sink();

        /** change the current log level.
         *
         * @param level new logging level
         */
        void set_log_level(log_levels level) {
            _level = level;
        };

        /** @return niveau courrant de journalisation
         */
        log_levels level() const {
            return _level;
        };

        /** @return logger name */
        const std::string &name() const {
            return _name;
        };

        /** @return program name */
        const std::string &program_name() const {
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

    protected:

        /** new class instance.
         *
         * > **WARN** these parameters are automatically passed when using the registry facility.
         *
         * @param name sink's name
         * @param pname program name
         * @param level initial log level (defaults to logger::log_level::info)
         */
        sink(const std::string &name = "default", const std::string &pname = "prog", log_level level = log_levels::info);

        /** @return display name for a given log level
         */
        std::string log_level_name(log_level level);

        std::string   _name; //!< logging domain name (as for now, this is equal to the logger name)
        std::string   _pattern;//!< message pattern (layout)
        std::string   _pname; //!< program name
        std::string   _ecid; //!< execution control ID. Helps to track everything that was logged by one business operation
        log_level     _level;    //!< current logging level

    private:
#if __cplusplus >= 201703L
        std::shared_mutex _shared_mutex; //!< used to protect access to sink's data
#else
        std::mutex _mutex;       //!< used to protect access to sink's data
#endif
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
         * @param name nom du journal
         * @param pname program name
         * @param file output file.
         * @param level initial log level (defaults to logger::log_levels::info)
         */
        file_sink(const std::string &name, const std::string &pname, log_level level, FILE *file);

        // virtual ~file_sink() ;

        /** \copydoc sink::write()
         *
         * This sink writes messages in FILE.
         *
         */
        virtual void write(log_level level, const char *fmt, ...) override ;

    protected:

        /** fill the buffer with the current date and time information
         */
        const std::string date_time();

        FILE             *_file_descriptor; //!< file descriptor of a log file
        pid_t             _pid;      //!< process ID
        std::string       _lag;      //!< date time lag (i.e. +02:00)
        std::string       _hostname; //!< hostname (this will be displayed by log messages)
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


    };

    /** syslog sink.
     *
     * send log messages to syslogd
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
         * @param facility_key syslog facility to use (default is "user")
         * @param options syslog options
         * @throws sink_exception if init went wrong
         */
        syslog_sink(const std::string &name, const std::string &pname, log_level level, const std::string &facility_key, int options);

       /** new instance.
         *
         * @param name nom du journal
         * @param pname program name
         * @param level initial log level (defaults to logger::log_levels::info)
         */
        syslog_sink(const std::string &name, const std::string &pname, log_level level);

        virtual ~syslog_sink();

        /** \copydoc  sink::write
         *
         * send messages to syslogd
         */
        virtual void write(log_level level, const char *fmt, ...) override ;
    };

    /** @} */

} // namespace logger
#endif
