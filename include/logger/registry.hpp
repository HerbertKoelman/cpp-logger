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
#include <logger.hpp>

#ifndef PMU_LOGGER_REGISTRY_HPP
#define PMU_LOGGER_REGISTRY_HPP

namespace pmu {
  namespace log {

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
        void remove(const std::string name);

        /** set the log level of all registered loggers
         *
         * @param level log level
         */
        void set_log_level ( log_level level );

        /** unregister all loggers
         */
        void reset();

        log_level level() const{
          return _level;
        }

        /** @return a logger instance (if not found a new one is created)
         */
        logger_ptr get(const std::string name);

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
        static std::tr1::unordered_map <std::string, logger_ptr> _loggers; //!< known loggers
#else
        static std::unordered_map <std::string, logger_ptr>      _loggers; //!< known loggers
#endif

        static pthread::mutex _mutex; //!< used to protect access to static class data
        static log_level      _level; //!< used when new logger instances are created by the regsitry

        static registry       _registry; //!< singleton
    };

    /** Searches the registry for the wanted logger instance.
     *
     * If the logger doesn't exist, then a new one is created and registered.
     *
     * @param name logger name
     * @return a logger instance
     */
    logger_ptr get (const std::string name );
    
  } // namespace log
} // namespace pmu
#endif
