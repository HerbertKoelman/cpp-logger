//
//  registry.cpp
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "logger/registry.hpp"

namespace pmu {
  namespace log {
    
    /** initailize static data
     */
    registry       registry::_registry = registry() ;
    
    logger_ptr get (const std::string &name ){

      return registry::instance().get(name);
    }

    void set_level(const log_level level){
      registry::instance().set_log_level(level);
    }

    void set_program_name(const std::string &pname){
      registry::instance().set_program_name(pname);
    }

    // Registry implementation ------------------------------------------------------
    //
    registry &registry::instance(){
      return _registry;
    }

    logger_ptr registry::get(const std::string &name){
      pthread::lock_guard<pthread::mutex> lck(_mutex);

      // printf("DEBUG pmu::log::registry.get(%s, %d);\n", name.c_str(), _level);

      logger_ptr logger;
      auto search = _loggers.find(name);

      if ( search == _loggers.end() ){
        logger = logger_ptr(new pmu::log::logger(name, _pname, _level));
        add(logger);
      } else {
        logger = search->second;
      }

      return logger;
    }

    void registry::set_program_name(const std::string &pname){
      _pname = pname ;
    }

    std::string registry::program_name(){
      return _pname;
    }

    void registry::set_log_level( const log_level level ){
      pthread::lock_guard<pthread::mutex> lck(_mutex);

      for (auto logger = _loggers.begin(); logger != _loggers.end(); ++logger) {
        logger->second->set_log_level(level);
      }

      _level = level;
      // printf("DEBUG registry log level is now %d\n", _level);
    }

    void registry::set_ecid( const std::string ecid){
      pthread::lock_guard<pthread::mutex> lck(_mutex);

      for (auto logger = _loggers.begin(); logger != _loggers.end(); ++logger) {
        logger->second->set_ecid(ecid);
      }

      // printf("DEBUG registry ecid is now %s\n", _ecid);
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

    registry::registry(): _level(log_levels::info), _pname("program") {
      // intientional...
    }

    registry::~registry(){
      // intientional...
    }

  } // namespace log
}   // namespace pmu
