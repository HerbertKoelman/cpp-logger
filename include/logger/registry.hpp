/*
 * pmu::log::registry - herbert koelman
 */

#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <unordered_map>

#include <pthread/pthread.hpp>
#include "logger/definitions.hpp"
#include <logger/logger.hpp>

#ifndef PMU_LOGGER_REGISTRY_HPP
#define PMU_LOGGER_REGISTRY_HPP

namespace pmu {
  namespace log {

   /** \addtogroup pmu_log
    * @{
    */

    /** Searches the registry for the wanted logger instance.
     *
     * If the logger doesn't exist, then a new one is created and registered.
     *
     * @param name logger name
     * @return a logger instance
     */
    logger_ptr get (const std::string &name );

    /** Set the current log level of all registered loggers.
     *
     * Default level becomes this level.
     *
     * @param level wanted log level.
     */
    void set_level(const log_level level);

    /** set program name.
     *
     * this name will be used for any logger created after this call.
     *
     * @param pname program name
     */
    void set_program_name(const std::string &pname);

    /** wraps a map of logger map
     */
    class registry {
      public:

        /** registry factory.
         */
        static registry &instance();

        /** unregister/remove an existing logger
         *
         * @param name name of the logger to unregister.
         */
        void remove(const std::string &name);

        /** set the log level of all registered loggers
         *
         * @param level log level
         */
        void set_log_level ( log_level level );

        void set_program_name(const std::string &pname);

        std::string program_name();

        /** unregister all loggers
         */
        void reset();

        /**  this is the log level that will be set when a new logger is instanciated.
         *
         * @return registry log level
         */
        log_level level() const{
          return _level;
        }

        /** set the ecid of all registered loggers
         *
         * @param ecid
         */
        void set_ecid ( const std::string ecid );


        /** @return a logger instance (if not found a new one is created)
         */
        logger_ptr get(const std::string &name);

        /**
         * @param name logger instance name
         * @param T a logger type
         * @param args logger type special arguments
         * @return a logger instance (if not found a new one is created)
         */
        template<class T, typename... Args> logger_ptr get( const std::string &name, const Args&... args){
          pthread::lock_guard<pthread::mutex> lck(_mutex);

          // printf("DEBUG pmu::log::registry.get(%s, %d);\n", name.c_str(), _level);

          logger_ptr logger;
          auto search = _loggers.find(name);

          if ( search == _loggers.end() ){
            logger = logger_ptr(new T(name, _pname, _level, args...));
            add(logger);
          } else {
            logger = search->second;
          }

          return logger;
        };

        ~registry();

      private:

        /** register/add a new logger instance
         *
         * @param logger logger instance
         */
        void add(logger_ptr logger);

        /** registry instance is a singleton and MUST be create through a factory.
         */
        registry();

#if __IBMCPP_TR1__ // NOSONAR this macro is set with the compiler command line argument
        std::tr1::unordered_map <std::string, logger_ptr> _loggers; //!< known loggers
#else
        std::unordered_map <std::string, logger_ptr>      _loggers; //!< known loggers
#endif

        pthread::mutex _mutex; //!< used to protect access to static class data
        log_level      _level; //!< used when new logger instances are created by the regsitry
        std::string    _pname;

        static registry       _registry; //!< singleton
    };

    /* IT IS not possible to forward declare method of a class. Therefore this function MUST implemented after the
     * registry's class definition.
     * This can be done either by having the code at the end of this file or to move the template into
     * another header file that starts with including the registry class definition.
     */

    /** Searches the registry for the wanted logger instance.
     *
     * If the logger doesn't exist, then a new one is created and registered.
     *
     * @param name logger instance name
     * @param T a logger type
     * @param args logger type special arguments
     * @return a logger instance
     */
    template<class T, typename... Args> logger_ptr get( const std::string &name, const Args&... args){
      return registry::instance().get<T>(name, args...);
    };

    /** @} */
  } // namespace log
} // namespace pmu
#endif
