//
// Created by Herbert Koelman on 2019-04-14.
//

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

#ifndef CPP_LOGGER_LOG_FACILITY_HPP
#define CPP_LOGGER_LOG_FACILITY_HPP

#include <logger/exceptions.hpp>
#include <logger/definitions.hpp>

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
            const facility_code code() const;

            /**
             * @return facility's keyword of the kind of program that is logging messages.
             */
            const std::string &keyword() const;

            /**
             *
             * @return facility's short description of what kind of program is logging messages.
             */
            const std::string &description() const;

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

/* TODO remove this sample const class
        class const_class {
        public:
            constexpr const char *thing() const{
                return _thing;
            }

            constexpr facility_code code() const{
                return _code;
            }

            void operator()() const {
                std::cout << _thing << "-" << _code << std::endl;
            }
            constexpr const_class(const char *thing, facility_code code): _thing(thing), _code(code){
                // intentional
            }
        private:
            const char *_thing;
            const facility_code _code;
        };

        constexpr const_class CONST_ONE{"const expression class one", facility_code::user};
        constexpr const_class CONST_TWO{"const expression class two", facility_code::uucp};
*/
//        constexpr facility user_facility{facility_code::user, "user", "User-level messages"};
//
//        /** Send facility description string to an output stream
//         *
//         * @param stream
//         * @param facility
//         * @return facility description string
//         */
//        std::ostream& operator<< (std::ostream& stream, const facility& facility);
    }
    /** @} */
}


#endif //CPP_LOGGER_LOG_FACILITY_HPP
