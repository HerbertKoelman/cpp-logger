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
            _level(level),
            _pname(pname){
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

    std::string sink::ecid() {
        std::shared_lock lock(_shared_mutex);

        return _ecid;
    }

    void sink::set_ecid(const std::string &ecid) {

        std::unique_lock lock(_shared_mutex);

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

} // namespace logger
