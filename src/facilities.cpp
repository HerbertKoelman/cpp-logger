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

    log_facility::log_facility(int code, const std::string &keyword, const std::string &description ) :
      _code(code),
      _keyword(keyword),
      _description(description)
    {
        // intentional...
    }

    log_facility::~log_facility() {
        // intentionale - std::cout << "destruction of facility instance: " << description() << std::endl ;
    }

    // syslog facility -------------------------------------------------------------------------------------------------

    namespace syslog {

        facility::facility(syslog::facility_code code, const std::string &keyword, const std::string &description)
                : log_facility((int) code, keyword, description) {
            // intentional...
        }

        facility_ptr facility::create_for(const std::string &key) {

            facility_ptr facility;

            if (key == "kern") {
                facility = std::make_shared<syslog::facility>(syslog::facility_code::kern, "kern", "Kernel messages");
            } else if (key == "user") {
                facility = std::make_shared<syslog::facility>(syslog::facility_code::user, "user", "User-level messages");
            } else if (key == "mail") {
                facility = std::make_shared<syslog::facility>(syslog::facility_code::mail, "mail", "Mail system");
            } else if (key == "daemon") {
                facility = std::make_shared<syslog::facility>(syslog::facility_code::daemon, "daemon", "System daemons");
            } else if (key == "auth") {
                facility = std::make_shared<syslog::facility>(syslog::facility_code::auth, "auth", "Security/authorization messages");
            } else if (key == "local0") {
                facility = std::make_shared<syslog::facility>(syslog::facility_code::local0, "local0", "Local use (local0)");
            } else if (key == "local1") {
                facility = std::make_shared<syslog::facility>(syslog::facility_code::local1, "local1", "Local use (local1)");
            } else {
                throw facility_exception("[" + key + "] is not a syslog key");
            }

            return facility;
        }

        facility_ptr facility::default_facility() {
            return std::make_shared<facility>();
        }

        facility::~facility() {
            // intentional - std::cout << "destruction of a facility" << std::endl;
        }
    } // namespace syslog
} // namespace logger