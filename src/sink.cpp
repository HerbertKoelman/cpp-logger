//
//  logger.cpp
//  substation
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "logger/sinks.hpp"

namespace logger {

    // abstract sink class -------------------
    //
    sink::sink(const std::string &name, const std::string &pname, log_level level) :
    // init class data
            _name(name),
            _pname(pname),
            _level(level){
        // intentional...
#ifdef DEBUG
        printf("DEBUG %s (%s,%d).\n", __FUNCTION__, __FILE__, __LINE__);
#endif

    }

    sink::~sink() {
#ifdef DEBUG
        printf("DEBUG %s (%s,%d).\n", __FUNCTION__, __FILE__, __LINE__);
#endif

        // intentional
    }

    void sink::set_log_level(log_levels level) {
        _level = level;
    };

    /** @return niveau courrant de journalisation
     */
    log_levels sink::level() {
        return _level;
    };

    std::string sink::ecid() {
#if __cplusplus >= 201703L
        std::shared_lock lock(_shared_mutex);
#else
        std::unique_lock(_mutex);
#endif

        return _ecid;
    }

    void sink::set_ecid(const std::string &ecid) {
#if __cplusplus >= 201703L
        std::unique_lock lock(_shared_mutex);
#else
        std::unique_lock lock(_mutex);
#endif

        if (!ecid.empty()) {
            _ecid = "[M ECID=\"" + ecid.substr(0, MAXECIDLEN) + "\"]";
        } else {
            _ecid = "- ";
        }
    }

    std::string sink::log_level_name(log_level level) {
        switch (level) {
            case log_level::emerg:
                return "EMERG";
            case log_level::alert:
                return "ALERT";
            case log_level::err:
                return "ERROR";
            case log_level::warning:
                return "WARNING";
            case log_level::notice:
                return "NOTICE";
            case log_level::info:
                return "INFO";
            case log_level::debug:
                return "DEBUG";
            case log_level::trace:
                return "TRACE";
            default:
                return "UNKNOWN";
        }

    }

    void sink::set_program_name(const std::string &name) {

        _pname = name;
    }

    void sink::set_name(const std::string &name) {

        _name = name;
    }

} // namespace logger
