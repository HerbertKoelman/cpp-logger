//
//  registry.cpp
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//


#include <pthread/pthread.hpp>
#include "logger/registry.hpp"

namespace pmu {
  namespace log {
    
    /** initailize static data
     */
    registry       registry::_registry = registry() ;
    log_level      registry::_level    = log_levels::info;
    pthread::mutex registry::_mutex;
    
#if __IBMCPP_TR1__
    std::tr1::unordered_map <std::string, logger_ptr> registry::_loggers;
#else
    std::unordered_map <std::string, logger_ptr>      registry::_loggers;
#endif
    
    logger_ptr get (const std::string &name ){

      return registry::instance().get(name);
    }

    // Registry implementation ------------------------------------------------------
    //
    registry &registry::instance(){
      return _registry;
    }

    logger_ptr registry::get(const std::string &name){
      pthread::lock_guard<pthread::mutex> lck(_mutex);

      // TODO remove this  printf("pmu::log::registry.get(%s);\n", name.c_str());

      logger_ptr logger;
      auto search = _loggers.find(name);

      if ( search == _loggers.end() ){
        logger = logger_ptr(new pmu::log::logger(name));
        add(logger);
      } else {
        logger = search->second;
      }

      return logger;
    }

    void registry::set_log_level( const log_level level ){
      pthread::lock_guard<pthread::mutex> lck(_mutex);

      for (auto logger = _loggers.begin(); logger != _loggers.end(); ++logger) {
        logger->second->set_log_level(level);
      }

      _level = level;
    }

    void registry::add(logger_ptr logger){
      // TODO remove this lock.
      // internal utility method doesn't have to be protected by a mutex
      // pthread::lock_guard<pthread::mutex> lck(_mutex);

      // check if the logger was already registered
      auto search = _loggers.find(logger->name()); 
      if ( search == _loggers.end() ){
        // add the logger in the map
        _loggers[logger->name()] = logger;
      }
    }

    void registry::remove(const std::string &name){
      pthread::lock_guard<pthread::mutex> lck(_mutex);

      _loggers.erase(name);
    }

    registry::registry(){
    }

    registry::~registry(){
    }

  } // namespace log
}   // namespace pmu
