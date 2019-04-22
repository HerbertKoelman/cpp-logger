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
        // intentionale - std::cout << "destruction of facility instance: " << description() << std::endl ;
    }

    // syslog facility -------------------------------------------------------------------------------------------------

    syslog_facility::syslog_facility(int code, std::string keyword, std::string description) : log_facility(code, keyword, description) {
        // intentional...
    }

    syslog_facility_ptr syslog_facility::create_for(const std::string &key){

        syslog_facility_ptr facility;

        if ( key == "kern" ){
        	facility = std::make_shared<syslog_facility>(0, "kern", "Kernel messages");
        } else if ( key == "user" ) {
	        facility = std::make_shared<syslog_facility>(1, "user", "User-level messages");
        } else if ( key == "mail" ) {
	        facility = std::make_shared<syslog_facility>(2, "mail", "Mail system");
        } else if ( key == "daemon" ) {
	        facility = std::make_shared<syslog_facility>(3, "daemon", "System daemons");
        } else if ( key == "auth" ) {
	        facility = std::make_shared<syslog_facility>(4, "auth", "Security/authorization messages");
        } else if ( key == "local0" ) {
	        facility = std::make_shared<syslog_facility>(16, "local0", "Local use (local0)");
        } else if ( key == "local1" ) {
	        facility = std::make_shared<syslog_facility>(17, "local1", "Local use (local1)");
        } else {
	        throw facility_exception("[" + key + "] is not a syslog key");
        }

        return facility;
    }

    syslog_facility_ptr syslog_facility::default_facility(){
        return std::make_shared<syslog_facility>();
    }

    syslog_facility::~syslog_facility() {
        // intentional - std::cout << "destruction of a syslog_facility" << std::endl;
    }
}