/*
 * logger - herbert koelman
 */

//#include "logger/config.h"

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
       * print a trace message.
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

      /** print a debug message.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::debug.
       */
      template<typename... Args> void debug( const std::string &fmt, const Args&... args){
          log(log_levels::debug, fmt, args...);
      };

      /** information message.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::info.
       */
      template<typename... Args> void info( const std::string &fmt, const Args&... args){
          log(log_levels::info, fmt, args...);
      };

      /** notice message
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::notice.
       */
      template<typename... Args> void notice( const std::string &fmt, const Args&... args){
          log(log_levels::notice, fmt, args...);
      };

      /** warning message.
       *
       * The system should continue to work.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations.
       * @param args data to print.
       * @see log handles the actual logging at log_levels::warning.
       */
      template<typename... Args> void warning( const std::string &fmt, const Args&... args){
          log(log_levels::warning, fmt, args...);
      };

      /** Error message.
       *
       * The system should try to recover. Maybe some fall back should be started.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::err.
       */
      template<typename... Args> void err( const std::string &fmt, const Args&... args){
          log(log_levels::err, fmt, args...);
      };

      /** Critical message.
       *
       * The system experiences critical conditions.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::crit.
       */
      template<typename... Args> void crit( const std::string &fmt, const Args&... args){
          log(log_levels::crit, fmt, args...);
      };

      /** Alert message.
       *
       * @tparam Args variadic of values to print.
       * @param fmt pointer to a null-terminated multibyte string specifying how to interpret the data. (see printf for more informations)
       * @param args data to print.
       * @see log handles the actual logging at log_levels::alert.
       */
      template<typename... Args> void alert( const std::string &fmt, const Args&... args){
          log(log_levels::alert, fmt, args...);
      };

      /** Emergency message.
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

      /** @return niveau courrant de journalisation
       */
      log_levels level() const ;

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

      /** @return logger name */
      std::string name() const;

      /** @return logger's facility (see logger::log_facility) */
      const std::string facility() const;

      /** modifie la facilit. . utiliser.
       *
       * @param facility facility to use
       */
      void set_facility(log_facility facility);

      /** instancie un objet pour journaliser
       *
       * @param name nom du journal
       * @param sink sink that this instance will delegate log writing
       */
      logger( const std::string &name, sink *sink);

      /** dispose of logger instance ressources
       */
      virtual ~logger();

    private:

      sink        *_sink; //!< logger delegate to a sink the actual magic to write log messages

    }; // logger

    /** @} */

} // namespace logger
#endif
