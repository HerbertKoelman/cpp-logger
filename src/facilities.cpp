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

//    log_facility::~log_facility() {
//        // intentionale - std::cout << "destruction of facility instance: " << description() << std::endl ;
//    }

    // syslog facility -------------------------------------------------------------------------------------------------

    namespace syslog {

        /** initialize static facilities singleton.
         *
         * This map avoids the need to recreate a facility instance and reuse existing ones.
         */
        // TODO switch to this as soon as possible - std::unordered_map<facility_code, facility_ptr> facility::_facilities;
        std::unordered_map<int, facility_ptr> facility::_facilities;

        facility::facility(syslog::facility_code code, const std::string &keyword, const std::string &description)
                : log_facility((int) code, keyword, description) {
            // intentional...
        }

        facility_ptr facility::create_for(const std::string &key) {

            facility_code code ;
            std::string keyword;
            std::string description;

            /* gather needed value to create a syslog::facility instance
             */
            if (key == "kern") {
                code = syslog::facility_code::kern;
                keyword = "kern";
                description = "Kernel messages";
            } else if (key == "user") {
                code = syslog::facility_code::user;
                keyword = "user";
                description = "User-level messages";
            } else if (key == "mail") {
                code = syslog::facility_code::mail;
                keyword = "mail";
                description = "Mail system";
            } else if (key == "daemon") {
                code = syslog::facility_code::daemon;
                keyword = "daemon";
                description = "System daemons";
            } else if (key == "auth") {
                code = syslog::facility_code::auth;
                keyword = "auth";
                description = "Security/authorization messages";
            } else if (key == "local0") {
                code = syslog::facility_code::local0;
                keyword = "local0";
                description = "Local use (local0)";
            } else if (key == "local1") {
                code = syslog::facility_code::local1;
                keyword = "local1";
                description = "Local use (local1)";
            } else {
                throw facility_exception("[" + key + "] is not a syslog key");
            }


            facility_ptr facility;
            auto search = _facilities.find(code);
            if (search == _facilities.end()) {
                // no facility found, need to create a new instance for code, key and description.
                facility = std::make_shared<syslog::facility>(code, keyword, description);

                // Save new instance
                _facilities[facility->code()] = facility; // saved new facility instance
            } else {
                facility = search->second;
            }

            return facility;
        }

        facility_ptr facility::default_facility() {
            return std::make_shared<facility>();
        }

//        facility::~facility() {
//            // intentional - std::cout << "destruction of a facility" << std::endl;
//        }
    } // namespace syslog
} // namespace logger