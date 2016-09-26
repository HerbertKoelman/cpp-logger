/*
 * pmu::logger - herbert koelman
 */

#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <unordered_map> // supposed to be faster
#include <pthread/pthread.hpp>
#include "logger/definitions.hpp"

#ifndef PMU_LOGGER_HPP
#define PMU_LOGGER_HPP

namespace pmu {
  namespace log {
   /** \addtogroup pmu_log
    * @{
    */

    class sink ;

    /** handles log messages.
     *
     * @author herbert koelman (herbert.koelman@pmu.fr)
     * @example logger-tests.cpp
     */
    class logger {
    public:

      /** affiche un message debug si le niveau de journalisation  >= log_levels::trace.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments sp.cifique . imprimer
       */
      template<typename... Args> void trace( const std::string &fmt, const Args&... args){
          log(log_levels::trace, fmt, args...);
      };

      /** affiche un message debug si le niveau de journalisation  >= log_levels::debug.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments sp.cifique . imprimer
       */
      template<typename... Args> void debug( const std::string &fmt, const Args&... args){
          log(log_levels::debug, fmt, args...);
      };

      /** affiche un message d'info si le niveau de journalisation est >= log_levels::info.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void info( const std::string &fmt, const Args&... args){
          log(log_levels::info, fmt, args...);
      };

      /** affiche un message notice si le niveau de journalisation est >= log_levels::notice.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void notice( const std::string &fmt, const Args&... args){
          log(log_levels::notice, fmt, args...);
      };

      /** affiche un message warning si le niveau de journalisation est >= log_levels::warning
       *
       * @param fmt chaine de formattage (see printf for more informations.
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void warning( const std::string &fmt, const Args&... args){
          log(log_levels::warning, fmt, args...);
      };

      /** affiche un message err si le niveau de journalisation est >= log_levels::err.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void err( const std::string &fmt, const Args&... args){
          log(log_levels::err, fmt, args...);
      };

      /** affiche un message crit si le niveau de journalisation est >= log_levels::crit.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void crit( const std::string &fmt, const Args&... args){
          log(log_levels::crit, fmt, args...);
      };

      /** affiche un message alert si le niveau de journalisation est  >= log_levels::alert.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void alert( const std::string &fmt, const Args&... args){
          log(log_levels::alert, fmt, args...);
      };

      /** affiche un message emerg si le niveau de journalisation est >= log_levels::emerg.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void emerg( const std::string &fmt, const Args&... args){
          log(log_levels::emerg, fmt, args...);
      };

      /** log a message if current log level is >= level
       *
       * @param level message logging level
       * @param fmt formatting string (see printf for more informations)
       * @param args message arguments
       */
      template<typename... Args> void log( log_level level, const std::string &fmt, const Args&... args){
        _sink->write(level, fmt.c_str(), args... );

//        if ( _level >= level) {
//
//          // we make a copy of the formatting string (fmt) because we receive a const string
//          // and want to change things in i
//          std::string format = fmt;
//
//          printf(
//                // first, we build a format pattern made of the current pattern (which will hold the prefix and the fmt received
//                (
//                  _pattern +
//                  (format.at(format.size()-1) == '\n' ? format : format += '\n') // handle string termination
//                ).c_str(),
//              level,
//              date_time().c_str(),
//              _hostname,
//              _facility.c_str(),
//              _pname.c_str(),
//              _pid,
//              pthread::this_thread::get_id(),
//              _ecid.empty()? "- " : _ecid.c_str(),
//              args...
//          );
//        }
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

      /** @return logger's facility (see pmu::log::log_facility) */
      const std::string facility() const;

      /** modifie la facilit. . utiliser.
       *
       * @param facility facility to use
       */
      void set_facility(log_facility facility);

      /** instancie un objet pour journaliser
       *
       * @param name nom du journal
       * @param level initial log level (defaults to pmu::log::info)
       */
      logger( const std::string &name, sink *sink);

      /** dispose of logger instance ressources
       */
      virtual ~logger();

    private:

      sink        *_sink; //!< logger delegate to a sink the actual magic to write log messages

    }; // logger

    /** @} */

  } // namespace log
} // namespace pmu
#endif
