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

#define MAXECIDLEN 64

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
        std::string name() const {
            return _name;
        };

        /** @return logger's facility (see logger::log_facility) */
        const log_facility_ptr facility() const {

            return _facility;
        };

        /** set faclity name
         *
         * @param facility facility name
         */
        void set_facility(log_facility_ptr facility);

        /** change the current ecid.
         *
         * Setting this to an empty string will stop logger to prin
         *
         * @param ecid new ecid
         */
        void set_ecid(const std::string &ecid);

        /** @return ecid courran
         */
        std::string ecid();

    protected:

        /** new class instance.
         *
         * @param name log name
         * @param pname program name
         * @param level initial log level (defaults to logger::log_level::info)
         */
        sink(const std::string &name = "default", const std::string &pname = "prog", log_level level = log_levels::info);

        /** new class instance.
         *
         * @param name log name
         * @param pname program name
         * @param facility logging facility
         * @param level initial log level (defaults to logger::log_level::info)
         */
        sink(const std::string &name = "default", const std::string &pname = "prog", log_facility_ptr facility = nullptr, log_level level = log_levels::info);

        /** @return display name for a given log level
         */
        std::string log_level_name(log_level level);

        std::string   _name; //!< logging domain name (as for now, this is equal to the logger name)
        std::string   _pattern;//!< message pattern (layout)
        std::string   _pname; //!< program name
        std::string   _ecid; //!< execution control ID. Helps to track everything that was logged by one business operation
        log_level     _level;    //!< current logging level

        log_facility_ptr _facility; //!< current facility class

    private:
        std::mutex _mutex;       //!< used to protect access to static class data
        std::shared_mutex _shared_mutex;

    }; // sink

    /** file sink.
     *
     * send log messages to FILE.
     *
     * > it is up to you to handle the file opening/closing.
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

        virtual ~file_sink();

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

        FILE *_file_descriptor; //!< file descriptor of a log file
        pid_t _pid;      //!< process ID
        std::string _lag;      //!< date time lag (i.e. +02:00)
        std::string _hostname; //!< hostname (this will be displayed by log messages)
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
         * @param name log name
         * @param pname program name
         * @param level initial log level (defaults to logger::log_levels::info)
         */
        stdout_sink(const std::string &name = "stdout", const std::string &pname = "prog",
                    log_level level = log_level::info);

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
        stderr_sink(const std::string &name = "stderr", const std::string &pname = "prog",
                    log_level level = log_level::info);

    };

    /** syslog sink.
     *
     * send log messages to syslogd
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
         * @param facility syslog facilty to use (default is "user")
         * @param options syslog options
         * @throws sink_exception if init went wrong
         */
        syslog_sink(const std::string &name = "syslog", const std::string &pname = "prog", log_level level = log_level::info, const std::string &facility_key = "user", int options = 0);

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
