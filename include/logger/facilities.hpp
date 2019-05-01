//
// Created by Herbert Koelman on 2019-04-14.
//

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <logger/exceptions.hpp>
#include <logger/definitions.hpp>

#ifndef CPP_LOGGER_LOG_FACILITY_HPP
#define CPP_LOGGER_LOG_FACILITY_HPP

namespace logger {
    /** \addtogroup logger_log
     * @{
     */

    namespace syslog {

        class facility;

        typedef std::shared_ptr<facility> facility_ptr; //!< a smart pointer to a syslog_facility

        /** RFC5424 log facility.
         *
         * This RFC is used by syslog implementations (more [here](https://en.wikipedia.org/wiki/Syslog#Facility).
         *
         * Factories are provided to create instances.
         *
         * @since 2.0.0
         * @author Herbert Koelman
         */
        class facility {
        public:

            /**
             * @return facility's code of what kind of program is logging messages.
             */
            const facility_code code();

            /**
             * @return facility's keyword of the kind of program that is logging messages.
             */
            const std::string &keyword();

            /**
             *
             * @return facility's short description of what kind of program is logging messages.
             */
            const std::string &description();

            /**
             * @return default logging facility to use with SysLog.
             */
            static facility_ptr default_facility();

            /** Create a facility for a given syslog key.
             *
             * This factory searches a map for a previously create instance. If one is found, it is returned. Otherwise a new instance
             * is created and saved for future reuse.
             *
             * @param key  facility's keyword
             * @return corresponding facility instance.
             */
            static facility_ptr create_for(const std::string &key);

            // TODO this should be protected or private (protected:)

            /** New syslog faiclity instance.
             *
             * @param code type of program that is logging the message (default is *1*)
             * @param keyword related keyword (default is *user*)
             * @param description falicity description.
             */
            explicit facility(facility_code code = facility_code::user, const std::string &keyword = "user", const std::string &description = "User-level messages");

            // virtual ~facility() ;

        private:
            // TODO switch to this as soon as possible static std::unordered_map<facility_code, facility_ptr> _facilities;
            static std::unordered_map<int, facility_ptr> _facilities; //!< map of share pointers to SysLog facility instances (mainly for reuse)

            facility_code _code;
            std::string   _keyword;
            std::string   _description;
        };
    }
    /** @} */
}


#endif //CPP_LOGGER_LOG_FACILITY_HPP
