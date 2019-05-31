/*
 * logger::registry - herbert koelman
 */

#include <memory> // std::unique_ptr, std::make_unique
#include <string> // std::string
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <unordered_map>

#include <mutex> // std::mutex

#include "logger/definitions.hpp"
#include <logger/logger.hpp>
#include <logger/sinks.hpp>
#include <logger/definitions.hpp>

#ifndef CPP_LOGGER_REGISTRY_HPP
#define CPP_LOGGER_REGISTRY_HPP

namespace logger {

   /** \addtogroup logger_log
    * @{
    */

    /** Searches the registry for the wanted logger instance.
     *
     * If the logger doesn't exist, then a new one is created and registered. Else the existing one is returned. The factory
     * create instances of stdout_sinks.
     *
     * @param name unique logger name
     * @return a logger instance
     * @see logger::stdout_sink
     */
    logger_ptr get (const std::string &name );

    /** Set the current log level of all registered loggers.
     *
     * Default level becomes this level.
     *
     * @param level wanted log level.
     */
    void set_level(const log_level level);

    /** set ECID for all registered loggers.
     *
     * @param ecid ECID
     */
    void set_ecid(const std::string &ecid );

    /** set program name.
     *
     * *WARN* This name will be used by loggers created after this call. Pre-existing ones are not affected.
     *
     * @param pname program name
     * @see logger::sink#program_name
     * @see logger::logger#program_name
     */
    void set_program_name(const std::string &pname);

    /**
     * Empty registry logger map.
     *
     * > **WARN** a all registered logger were removed from the registry map.
     *
     * @see logger::registry#reset()
     */
    void reset_registry() ;

    /** The registry is a map of loggers.
     *
     * It makes it possible to re-use pre-existing loggers. each logger is indexed by it's name and type (sink). When a
     * multi-threaded programmed needs to log things, sharing a logger between threads might be helpfull.
     *
     * @author Herbert Koelman
     */
    class registry {
      public:

        /** @return registry singleton
         */
        static registry &instance() ;

        /** unregister/remove an existing logger
         *
         * @param name name of the logger to unregister.
         */
        void remove(const std::string &name);

        /** unregister all loggers
         */
        void reset();

        /** return number of registered loggers */
        size_t size() const {
          return _loggers.size();
        };

        /** set the log level of all registered loggers
         *
         * @param level log level
         */
        void set_log_level ( log_level level );

        /**  this is the log level that will be set when a new logger is instanciated.
         *
         * @return registry log level
         */
        log_level level() const{
          return _level;
        }

        /** set program name
         *
         * @param pname program name
         */
        void set_program_name(const std::string &pname);

        /** @return current program name
         */
        std::string program_name();

        /** set the ecid of all registered loggers
         *
         * @param ecid
         */
        void set_ecid ( const std::string &ecid );

        /** @return a logger instance that uses stdout (if not found a new one is created)
         *
         * @see stdout_sink send messages on stdout.
         */
        logger_ptr get(const std::string &name){
          return get<stdout_sink>(name);
        }

        /**
         * @tparam T logger's sink type
         * @tparam Args logger's sink argument variadic
         * @param name logger instance name
         * @param args logger's sink arguments
         * @return a logger instance (if not found a new one is created)
         * @see sink interface between the logger and the target output
         */
        template<class T, typename... Args> logger_ptr get( const std::string &name, const Args&... args){
          std::lock_guard<std::mutex> lck(_mutex);

          // printf("DEBUG logger::registry.get(%s, %d);\n", name.c_str(), _level);

          logger_ptr logger;
          auto search = _loggers.find(name);

          if ( search == _loggers.end() ){
            // no logger was registered yet, instantiate a new one.
            sink *sink = new T(args...);

            // we can setup these properties because regisrty and sink classes are friends.
            sink->set_name(name);
            sink->set_program_name(_pname);
            sink->set_log_level(_level);

            logger = logger_ptr(
              new class logger(
                name,
                sink
              )
            );

            // register the newly created logger instance
            add(logger);
          } else {
            logger = search->second;
          }

          return logger;
        };

        ~registry() = default;
        // ~registry() { std::cout << "DEBUG delete registry..." << std::endl ; };

      private:

        /** register/add a new logger instance
         *
         * **WARN** we don't need to protect the map access beacause it's done by the only method that accesses this one.
         *
         * @param logger logger instance
         * @see get<>
         */
        void add(const logger_ptr &logger);

        /** registry instance is a singleton and MUST be create through a factory.
         */
        registry() noexcept ;

#if __IBMCPP_TR1__ // NOSONAR this macro is set with the compiler command line argumen
        std::tr1::unordered_map <std::string, logger_ptr> _loggers; //!< known loggers
#else
        std::unordered_map <std::string, logger_ptr>      _loggers; //!< known loggers
#endif

        std::mutex     _mutex; //!< used to protect access to static class data
        log_level      _level; //!< used when new logger instances are created by the regsitry
        std::string    _pname;

        static std::unique_ptr<registry>       _registry; //!< singleton
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
     * @tparam T logger sink type
     * @tparam args sink's constructor argument variadic
     * @param name logger instance name
     * @param args sink's constructor arguments
     * @return a logger instance
     * @see logger::sink
     */
    template<class T, typename... Args> logger_ptr get( const std::string &name, const Args&... args){
      return registry::instance().get<T>(name, args...);
    };

    /** @} */
} // namespace logger
#endif
