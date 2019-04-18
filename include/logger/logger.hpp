/*
 * logger - herbert koelman
 */

#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <unordered_map> // supposed to be faster
#include "logger/definitions.hpp"
#include "logger/sinks.hpp"

#ifndef CPP_LOGGER_LOGGER_HPP
#define CPP_LOGGER_LOGGER_HPP

namespace logger {
   /** \addtogroup logger_log
    * @{
    */

    class sink ;

    /** handles log messages.
     *
     * @author herbert koelman
     */
    class logger {
    public:

      /**
       * Print a trace message.
       *
       * Messages that contain information normally of use only when debugging a program.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       *
       * @see log handles the actual logging at log_levels::trace.
       */
      template<typename... Args> void trace( const std::string &fmt, const Args&... args){
          log(log_levels::trace, fmt, args...);
      };

      /** Print a debug message.
       *
       * Messages that contain information normally of use only when debugging a program.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::debug.
       */
      template<typename... Args> void debug( const std::string &fmt, const Args&... args){
          log(log_levels::debug, fmt, args...);
      };

      /** Informational messages.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::info.
       */
      template<typename... Args> void info( const std::string &fmt, const Args&... args){
          log(log_levels::info, fmt, args...);
      };

      /** Normal but significant conditions
       *
       * Conditions that are not error conditions, but that may require special handling.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::notice.
       */
      template<typename... Args> void notice( const std::string &fmt, const Args&... args){
          log(log_levels::notice, fmt, args...);
      };

      /** Warning conditions.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations.
       * @param args data to print.
       * @see log handles the actual logging at log_levels::warning.
       */
      template<typename... Args> void warning( const std::string &fmt, const Args&... args){
          log(log_levels::warning, fmt, args...);
      };

      /** Error conditions.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::err.
       */
      template<typename... Args> void err( const std::string &fmt, const Args&... args){
          log(log_levels::err, fmt, args...);
      };

      /** Critical conditions.
       *
       * The system experiences critical conditions like hard device errors.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::crit.
       */
      template<typename... Args> void crit( const std::string &fmt, const Args&... args){
          log(log_levels::crit, fmt, args...);
      };

      /** Alert conditions.
       *
       * Action must be taken immediately. A condition that should be corrected immediately, such as a corrupted system database.[
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::alert.
       */
      template<typename... Args> void alert( const std::string &fmt, const Args&... args){
          log(log_levels::alert, fmt, args...);
      };

      /** Emergency conditions.
       *
       * System is unusable. A panic condition.
       * 
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::emerg.
       */
      template<typename... Args> void emerg( const std::string &fmt, const Args&... args){
          log(log_levels::emerg, fmt, args...);
      };

      /** log a message if current log level is >= level.
       *
       * @tparam Args variadic of values to print.
       * @param level message logging level
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       */
      template<typename... Args> void log( log_level level, const std::string &fmt, const Args&... args){
        _sink->write(level, fmt.c_str(), args... );
      };

      // -------------------------------------------------------------
      //

      /** change the current log level.
       *
       * @param level new logging level
       */
      void set_log_level( log_levels level );

      /** @return current log level.
       */
      log_levels level() const ;

      /** change the current ecid (execution content ID).
       *
       * Setting this to an empty string will deactivate the printing of ECIDs.
       *
       * @param ecid new ecid
       */
      void set_ecid( const std::string &ecid );

      /** @return current tracking ecid (execution content ID).
       */
      std::string ecid() ;

      /** @return logger name */
      std::string name() const;

      /** @return logger's facility (see logger::log_facility) */
      const log_facility_ptr facility() const;

//      /** modifie la facilit. . utiliser.
//       *
//       * @param facility facility to use
//       */
//      TODO check if we should allow to change logger's facility or if this can only be done once when a logger instance is created
//      void set_facility(log_facility *facility);

      /** create a logger instance.
       *
       * @param name logger name
       * @param sink sink we want this logger to use
       */
      logger( const std::string &name, sink *sink);

      /** dispose of logger's ressources
       */
      virtual ~logger();

    private:

      sink        *_sink; //!< logger delegate to a sink the actual magic to write log messages

    }; // logger

    /** @} */

} // namespace logger
#endif
