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
        log_facility(int code, std::string keyword, std::string description = "");

        virtual ~log_facility();

    private:
        int _code;
        std::string _keyword;
        std::string _description;
    };

    typedef std::shared_ptr<log_facility> log_facility_ptr; //!< a smart pointer to a log_facility

    class syslog_facility;

    typedef std::shared_ptr<syslog_facility> syslog_facility_ptr; //!< a smart pointer to a syslog_facility

    /** RFC5424 log facility.
     *
     * This RFC is used by syslog implementations (more [here](https://en.wikipedia.org/wiki/Syslog#Facility).
     *
     * Factories are provided to create instances.
     *
     * @since 2.0.0
     * @author Herbert Koelman
     */
    class syslog_facility : public log_facility {
    public:
        /**
         * @return default logging facility to use with SysLog.
         */
        static syslog_facility_ptr default_facility();

        /** Create the key related faiclity.
         *
         * @param key  facility's keyword
         * @return corresponding facility instance.
         */
        static syslog_facility_ptr create_for(const std::string &key);

        // TODO this should be protected or private (protected:)

        /** New syslog faiclity instance.
         *
         * @param code type of program that is logging the message (default is *1*)
         * @param keyword related keyword (default is *user*)
         * @param description falicity description.
         */
        syslog_facility(facility_code code = facility_code::user, std::string keyword = "user", std::string description = "User-level messages");

        virtual ~syslog_facility();
    };

    /** @} */
}


#endif //CPP_LOGGER_LOG_FACILITY_HPP
