//
//  logger.cpp
//  substation
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "logger/logger.hpp"
#include "logger/sinks.hpp"

namespace logger {

    // constructors & destructors -------------------------------------
    //

    logger::logger(const std::string &name, sink *sink) : _sink(NULL) {
        _sink = sink;
    }

    logger::~logger() {
        // printf("DEBUG %s destructor (%s,%d).\n", __FUNCTION__, __FILE__, __LINE__);
        if (_sink != NULL) {
            delete _sink;
        }
    }

// TODOD remove this
//    const log_facility_ptr logger::facility() const {
//        return _sink->facility();
//    };

//    void logger::set_facility(log_facility *facility) {
//        _sink->set_facility(facility);
//    }

    std::string logger::ecid() {
        return _sink->ecid();
    }

    void logger::set_ecid(const std::string &ecid) {
        _sink->set_ecid(ecid);
    }


    /** change the current log level.
     *
     * @param level new logging level
     */
    void logger::set_log_level(log_levels level) {
        _sink->set_log_level(level);
    };

    /** @return niveau courrant de journalisation
     */
    log_levels logger::level() const {
        return _sink->level();
    };

    /** @return logger name */
    std::string logger::name() const {
        return _sink->name();
    };

} // namespace logger
