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

#ifndef PMU_LOGGER_HPP
#define PMU_LOGGER_HPP

#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7

#define PMU_LOG_PATTERN "<%d> %s [%s] (pid: %d, thrdid: %d) - "

namespace pmu {
  namespace log {

    class logger;

    enum log_levels {
      emerg   = LOG_EMERG, //!< system is unusable
      alert   = LOG_ALERT, //!< action must be taken immediately
      crit    = LOG_CRIT,  //!< critical conditions
      err     = LOG_ERR,   //!< error conditions
      warning = LOG_WARNING, //!< warning condition
      notice  = LOG_NOTICE,  //!< normal but signification condition
      info    = LOG_INFO,  //!< informational (default)
      debug   = LOG_DEBUG  //!< debug information
    };

    /** used when defined as a parameter */
    typedef log_levels log_level;

    enum log_facilities {
      sic_bat, //!< pour les batchs
      sic_ine, //!< pour les interfaces externes
      sic_bmp, //!< Pour les BMP IMS
      sic_mpp, //!< Pour le MPP IMS
      sic_kix, //!< Pour le CICS
      sic_tux  //!< (non encore utilis.)
    };

    typedef log_facilities log_facility;

#if __IBMCPP_TR1__
    typedef std::tr1::shared_ptr<pmu::log::logger> logger_ptr; //!< shared pointer to a logger instance
#else
    typedef std::shared_ptr<pmu::log::logger>      logger_ptr; //!< shared pointer to a logger instance
#endif

    /** Searches the registry for the wanted logger instance.
     *
     * If the logger doesn't exist, then a new one is created and registered.
     *
     * @param name logger name
     * @return a logger instance
     */
    logger_ptr get (const std::string name );

    /** Impl.mentation C++ de la fonctionnalit. 
     *
     * Ci-apr.s un exemple d'utilisation.
     *
     * <pre><code>
     * ...
     * logger log;
     * log.set_level(log_levels::debug);
     * log.debug("DEBUG decimal: %d %ld [%10d], different radices:%d %x %o %#x %#o, Padding: [%15s] [%-15s]", 1977, 650000L, 1977,100, 100, 100, 100, 100, "right", "left");
     * log.info("INFO decimal: %d %ld [%10d], different radices:%d %x %o %#x %#o, Padding: [%15s] [%-15s]", 1977, 650000L, 1977,100, 100, 100, 100, 100, "right", "left");
     * ...
     * </pre></code>
     *
     * @author herbert koelman (herbert.koelman@pmu.fr)
     */
    class logger {
    public:
      
      /** affiche un message debug si le niveau de journalisation  >= log_levels::debug.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments sp.cifique . imprimer
       */
      template<typename... Args> void debug( const std::string fmt, const Args&... args){
          log(log_levels::debug, fmt, args...);
      };

      /** affiche un message d'info si le niveau de journalisation est >= log_levels::info.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void info( const std::string fmt, const Args&... args){
          log(log_levels::info, fmt, args...);
      };

      /** affiche un message notice si le niveau de journalisation est >= log_levels::notice.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void notice( const std::string fmt, const Args&... args){
          log(log_levels::notice, fmt, args...);
      };

      /** affiche un message warning si le niveau de journalisation est >= log_levels::warning
       *
       * @param fmt chaine de formattage (see printf for more informations.
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void warning( const std::string fmt, const Args&... args){
          log(log_levels::warning, fmt, args...);
      };

      /** affiche un message err si le niveau de journalisation est >= log_levels::err.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void err( const std::string fmt, const Args&... args){
          log(log_levels::err, fmt, args...);
      };

      /** affiche un message crit si le niveau de journalisation est >= log_levels::crit.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void crit( const std::string fmt, const Args&... args){
          log(log_levels::crit, fmt, args...);
      };

      /** affiche un message alert si le niveau de journalisation est  >= log_levels::alert.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void alert( const std::string fmt, const Args&... args){
          log(log_levels::alert, fmt, args...);
      };

      /** affiche un message emerg si le niveau de journalisation est >= log_levels::emerg.
       *
       * @param fmt chaine de formattage (see printf for more informations)
       * @param args arguments specifying data to print.
       */
      template<typename... Args> void emerg( const std::string fmt, const Args&... args){
          log(log_levels::emerg, fmt, args...);
      };

      /** log a message if current log level is >= level
       * 
       * @param log level 
       */
      template<typename... Args> void log( log_level level, const std::string fmt, const Args&... args){
        if ( _level >= level) {

          // we make a copy of the formatting string (fmt) because we receive a const string 
          // and want to change things in it
          std::string _format = fmt; 

          printf(
                // first, we build a format pattern made of the current pattern (which will hold the prefix and the fmt received
                (
                  _pattern +
                  (_format.at(_format.size()-1) == '\n' ? _format : _format += '\n') // handle string termination
                ).c_str(),
              _level,
              date_time().c_str(),
              _facility.c_str(),
              _pid,
              pthread::this_thread::get_id(),
              args...
          ); 
        }
      };

      /** instancie un objet pour journaliser
       *
       * @param name nom du journal (imprim. ente [] si pr.sent)
       */
      logger( const std::string &name = "default", log_level = log_levels::info );

      /** dispose of logger instance ressources
       */
      virtual ~logger();
      
      /** modifie le niveau de journalisation courrant
       *
       * @param level nouveau niveau de journalisation
       */
      void set_log_level( log_levels level ){
        _level = level;
      };

      /** @return niveau courrant de journalisation
       */
      log_levels level() const {
         return _level;
      };
      
      std::string pattern() const{
        return _pattern;
      };

      /** @return logger name */
      std::string name() const{
        return _name;
      };

      const std::string facility() const {
        return _facility;
      };

      /** modifie la facilit. . utiliser.
       *
       * @param facility la facilit. . utiliser
       */
      void set_facility(log_facility facility);

    private:

      /** fill the buffer with the current date and time information
       */
      const std::string date_time();

      /** d�finit le pr�fix � utiliser
       *
       * @param pattern d.sir. (default "%s")
       */
      void set_pattern(std::string pattern);

      /** set the logger name.
       *
       * @param name logger name (i.e. some/thing)
       */
       void set_name(const std::string name);

      std::string  _name ;
      std::string  _pattern;
      std::string  _facility;
      log_level    _level;
      pid_t        _pid;
    }; // logger
    
  } // namespace log
} // namespace pmu
#endif
