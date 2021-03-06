//
//  registry.cpp
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "logger/registry.hpp"

namespace logger {

    /** singleton
     */
    std::unique_ptr<registry>  registry::_registry{new registry()};

    logger_ptr get(const std::string &name) {

        return registry::instance().get(name);
    }

    void set_level(const log_level level) {
        registry::instance().set_log_level(level);
    }

    void set_ecid(const std::string &ecid) {
        registry::instance().set_ecid(ecid);
    }

    void set_program_name(const std::string &pname) {
        registry::instance().set_program_name(pname);
    }

    void reset_registry() {
        registry::instance().reset();
    }

    // Registry implementation ------------------------------------------------------
    //
    registry &registry::instance() {
        return *_registry;
    }

    void registry::set_program_name(const std::string &pname) {
        _pname = pname;
    }

    std::string registry::program_name() {
        return _pname;
    }

    void registry::set_log_level(const log_level level) {
        std::lock_guard<std::mutex> lck(_mutex);

        for (auto & _logger : _loggers) {
            _logger.second->set_log_level(level);
        }

        _level = level;

#ifdef DEBUG
        printf("DEBUG registry logger level is now %d\n", _level);
#endif
    }

    void registry::set_ecid(const std::string &ecid) {
        std::lock_guard<std::mutex> lck(_mutex);

        for (auto & _logger : _loggers) {
            _logger.second->set_ecid(ecid);
        }

#ifdef DEBUG
        printf("DEBUG %s: registry ecid is now %s (%s,%d)\n", __FUNCTION__, ecid.c_str(), __FILE__, __LINE__);
#endif
    }

    void registry::add(const logger_ptr &logger) {
        // we don(t need to protect the map access beacause it's done by the only method that accesses
        // this one.

        // check if the logger was already registered
        auto search = _loggers.find(logger->name());
        if (search == _loggers.end()) {
            // add the logger in the map
            _loggers[logger->name()] = logger;
        }
    }

    void registry::remove(const std::string &name) {
        std::lock_guard<std::mutex> lck(_mutex);

        _loggers.erase(name);
    }

    void registry::reset() {
        std::lock_guard<std::mutex> lck(_mutex);
        _loggers.clear();
    }

    registry::registry() noexcept : _level(log_levels::info), _pname("program") {
        // intentional...
#       ifdef DEBUG
        std::cout << "DEBUG Create regsistry instance..." << std::endl;
#       endif
    }

} // namespace logger
