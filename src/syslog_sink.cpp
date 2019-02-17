//
//  Created by herbert koelman on Mon Oct  3 17:52:41 CEST 2016
//  Copyright © 2016 PMU. All rights reserved.
//

#include "logger/sinks.hpp"
#include <syslog.h>


namespace log {

    syslog_sink::syslog_sink(const std::string &name, const std::string &pname, log_level level, int facility, int options) :
            sink(name, pname, level) {
#ifdef DEBUG
        printf("DEBUG %s name: %s, pname: %s, level: %d, facility: %d, options: %d\n", __FUNCTION__,
            name.c_str(),
            pname.c_str(),
            level,
            facility,
            options,
            __FILE__,__LINE__);
#endif
        _pattern = "[L SUBSYS=" + _name + "] %s %s";

        if (options == 0) {
            options = LOG_PID;
        }

        if (facility == 0) {
            facility = LOG_USER;
        }

#ifdef DEBUG
        printf("DEBUG %s calling openlog(%s, %d, %d)\n",
            __FUNCTION__,
            pname.c_str(),
            options,
            facility,
            __FILE__,__LINE__);
#endif

        openlog(pname.c_str(), options, facility);

    };

    syslog_sink::~syslog_sink() {
        // intentional
    };

    void syslog_sink::write(log_level level, const char *fmt, ...) {
#ifdef DEBUG
        printf("DEBUG %s _level/level: %d/%d, pattern: [%s] (%s,%d)\n", __FUNCTION__, _level, level, _pattern.c_str(), __FILE__,__LINE__);
#endif

        if (_level >= level) {

            // fill a buffer with the user message
            va_list args1;
            va_list args2;

            va_start(args1, fmt); // initialize va_list
            va_copy(args2, args1); // save a copy args1 -> args2

            // this call only returns the actual number of bytes needed to store the message.
            // It doesn't count the needed end-of-string
            size_t buffer_size = vsnprintf(NULL, 0, fmt, args1) + 1; // plus 1 character to store \0
            va_end(args1); // don't need this one anymore

            // add one more character to handle \n (see code below)
            char buffer[buffer_size + 1];
            memset(buffer, 0, buffer_size + 1);

            // fill buffer with message ...
            vsnprintf(buffer, buffer_size, fmt, args2);
            size_t len = strlen(buffer);
            va_end(args2);

            // add a new line if not already there
            if (buffer[len - 1] != '\n') {
                buffer[len] = '\n'; // override ending \0 (that's why we provisioned for one more character above)
            }

            auto syslog_level = LOG_INFO;
            switch (level) {
                case log_level::crit:
                    syslog_level = LOG_CRIT;
                    break;
                case log_level::emerg:
                    syslog_level = LOG_EMERG;
                    break;
                case log_level::alert:
                    syslog_level = LOG_ALERT;
                    break;
                case log_level::err:
                    syslog_level = LOG_ERR;
                    break;
                case log_level::warning:
                    syslog_level = LOG_WARNING;
                    break;
                case log_level::notice:
                    syslog_level = LOG_NOTICE;
                    break;
                case log_level::info:
                    syslog_level = LOG_INFO;
                    break;
                case log_level::debug:
                case log_level::trace:
                    syslog_level = LOG_DEBUG;
                    break;
                default:
                    syslog_level = LOG_INFO;
            }

            syslog(
                    syslog_level,
                    _pattern.c_str(),
                    _ecid.empty() ? "" : _ecid.c_str(),
                    buffer);
        }
    }; // write
} // namespace log
