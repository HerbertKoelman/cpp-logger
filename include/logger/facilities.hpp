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

        /** Represent a RFC5424 log facility.
         *
         * This RFC is used by syslog implementations (more [here](https://en.wikipedia.org/wiki/Syslog#Facility)).
         *
         * This is class is used to create constant expressions. That means that instances are created once at compile-time
         *
         * @since 2.0.0
         * @author Herbert Koelman
         */
        class facility {
        public:

            /**
             * @return facility's code of what kind of program is logging messages.
             */
            constexpr facility_code code() const {
                return _code;
            }

            /**
             * @return facility's keyword of the kind of program that is logging messages.
             */
            constexpr const char *keyword() const {
                return _keyword;
            }

            /**
             *
             * @return facility's short description of what kind of program is logging messages.
             */
            constexpr const char *description() const {
                return _description;
            }

            /** New syslog faiclity instance.
             *
             * @param code type of program that is logging the message (default is *1*)
             * @param keyword related keyword (default is *user*)
             * @param description falicity description.
             */
            constexpr facility(facility_code code, const char *keyword, const char *description )
              : _code(code), _keyword(keyword), _description(description) {
                // intentional...
            }

            // virtual ~facility() ;

        private:
            const facility_code _code;
            const char   *_keyword;
            const char   *_description;
        };

        /* Create constant standard facility instances */
        constexpr facility kern_facility{facility_code::kern,     "kern", "Kernel messages"};     //!< kernel facility
        constexpr facility user_facility{facility_code::user,     "user", "User-level messages"}; //!< user facility
        constexpr facility mail_facility{facility_code::mail,     "mail", "Mail system"};         //!< used to log mail related issues
        constexpr facility daemon_facility{facility_code::daemon, "daemon", "System daemons"};    //!< daemon facility
        constexpr facility auth_facility{facility_code::auth,     "auth", "Security/authorization messages"}; //!< used to log authentication issues
        constexpr facility syslog_facility{facility_code::syslog, "syslog", "Messages generated internally by syslogd"}; //!< syslog internal messages (should probably nerver be used)
        constexpr facility lpr_facility{facility_code::lpr,       "lpr", "Line printer subsystem"}; //!< used to report printing related issues.
        constexpr facility news_facility{facility_code::news,     "news", "Network news subsystem"}; //!< user to report network news related issues.
        constexpr facility uucp_facility{facility_code::uucp,     "uucp", "UUCP subsystem"}; //!< Unix-toUnix related issues (probably too old to make sense today)
        constexpr facility cron_facility{facility_code::cron,     "cron", "Clock daemon"};   //!< cron related issues
        constexpr facility authpriv_facility{facility_code::authpriv, "authpriv", "Security/authentication messages"};
        constexpr facility ftp_facility{facility_code::ftp,       "ftp", "FTP daemon"}; //!< file transfert related issues
        constexpr facility ntp_facility{facility_code::ntp,       "ntp", "NTP subsystem"}; //!< network time related issues
        constexpr facility security_facility{facility_code::security, "security", "Log audit"}; //!< security related issue
        constexpr facility console_facility{facility_code::console,   "console", "Log alert"}; //!< log something on the console

        /* Create constant standard freee to use facility instances */
        constexpr facility local0_facility{facility_code::local0, "local0", "Local use (local0)"};
        constexpr facility local1_facility{facility_code::local1, "local1", "Local use (local1)"};

        /** Send facility description string to an output stream
         *
         * @param stream
         * @param facility
         * @return facility description string
         */
        std::ostream& operator<< (std::ostream& stream, const facility& facility);
    }
    /** @} */
}


#endif //CPP_LOGGER_LOG_FACILITY_HPP
