//
// Created by Herbert Koelman on 2019-04-14.
//

#include <string>
#include <memory>
#include <iostream>
#include <logger/exceptions.hpp>
#include <logger/definitions.hpp>

#ifndef CPP_LOGGER_LOG_FACILITY_HPP
#define CPP_LOGGER_LOG_FACILITY_HPP

namespace logger {
    /** \addtogroup logger_log
     * @{
     */

    /** log facility abstraction.
     *
     * A facility is used to specify the type of program that is logging the message.
     *
     * @since 2.0.0
     * @author Herbert Koelman
     */
    class log_facility {
    public:
        /**
         * @return facility's code of what kind of program is logging messages.
         */
        const int code();

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
         * @param code type of program that is logging the message
         * @param keyword related keyword
         * @param description falicity description.
         */
        log_facility(int code, const std::string &keyword, const std::string &description = "");

        virtual ~log_facility();

    private:
        int _code;
        std::string _keyword;
        std::string _description;
    };

    typedef std::shared_ptr<log_facility> log_facility_ptr; //!< a smart pointer to a log_facility

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
        class facility : public ::logger::log_facility {
            public:
                /**
                 * @return default logging facility to use with SysLog.
                 */
                static facility_ptr default_facility();

                /** Create the key related faiclity.
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
                facility(facility_code code = facility_code::user, const std::string &keyword = "user", const std::string &description = "User-level messages");

                virtual ~facility();
            };
    }
    /** @} */
}


#endif //CPP_LOGGER_LOG_FACILITY_HPP
