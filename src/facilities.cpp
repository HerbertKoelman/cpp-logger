//
// Created by Herbert Koelman on 2019-04-14.
//

#include "logger/facilities.hpp"

namespace logger {

    const int log_facility::code() {
        return _code;
    }

    const std::string &log_facility::keyword() {
        return _keyword;
    }

    const std::string &log_facility::description() {
        return _description;
    }

    log_facility::log_facility(int code, std::string keyword, std::string description ) : _code(code), _keyword(keyword),
                                                                                _description(description) {
        // intentional...
    }

    log_facility::~log_facility() {
        std::cout << "destruction of facility instance: " << description() << std::endl ;
    }

    // syslog facility -------------------------------------------------------------------------------------------------

    syslog_facility::syslog_facility(int code, std::string keyword, std::string description) : log_facility(code, keyword, description) {
        // intentional...
    }

    syslog_facility_ptr syslog_facility::kern_facility(){
        return std::make_shared<syslog_facility>(0, "kern", "Kernel messages");
    }

    syslog_facility_ptr syslog_facility::default_facility(){
        return std::make_shared<syslog_facility>();
    }

    syslog_facility::~syslog_facility() {
        std::cout << "destruction of a syslog_facility" << std::endl;
    }
}