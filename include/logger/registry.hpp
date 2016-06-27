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

        /** @return a logger instance (if not found a new one is created)
         */
        logger_ptr get(const std::string &name);

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

#if __IBMCPP_TR1__
        std::tr1::unordered_map <std::string, logger_ptr> _loggers; //!< known loggers
#else
        std::unordered_map <std::string, logger_ptr>      _loggers; //!< known loggers
#endif

        pthread::mutex _mutex; //!< used to protect access to static class data
         log_level      _level; //!< used when new logger instances are created by the regsitry

        static registry       _registry; //!< singleton
    };
    
    /** @} */
  } // namespace log
} // namespace pmu
#endif
