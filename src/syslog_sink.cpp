//
//  Created by herbert koelman on Mon Oct  3 17:52:41 CEST 2016
//  Copyright © 2016 Urbix. All rights reserved.
//

#include "logger/sinks.hpp"
#include <syslog.h>
#include <cstring> // std::memset

namespace logger {

    syslog_sink::syslog_sink(const syslog::facility &facility, int options) :
            syslog_sink("default", "pname", log_level::info, facility, options){

    }

    syslog_sink::syslog_sink() :
            syslog_sink("default", "pname", log_level::info, syslog::user_facility, 0){
        // intentional...
    };

    syslog_sink::syslog_sink(const std::string &name, const std::string &pname, log_level level, const syslog::facility &facility, int options):
            sink(name, pname, level) {

        try {
#ifdef DEBUG
            printf("DEBUG %s name: %s, pname: %s, level: %d, facility: %d, options: %d\n", __FUNCTION__,
                name.c_str(),
                pname.c_str(),
                level,
                facility,
                options,
                __FILE__,__LINE__);
#endif
            set_name(name);

            if (options == 0) {
                options = LOG_PID;
            }

#ifdef DEBUG
            printf("DEBUG %s calling openlog(%s, %d, %d)\n",
                __FUNCTION__,
                pname.c_str(),
                options,
                facility,
                __FILE__,__LINE__);
#endif

            openlog(program_name().c_str(), options, (int)facility.code());

        } catch ( std::exception &err ){
            throw logger_exception(err.what());
        }
    };

    syslog_sink::syslog_sink(const std::string &name, const std::string &pname, log_level level) :
            syslog_sink(name, pname, level, syslog::user_facility, 0) {
        // intentional
    }

    void syslog_sink::write(log_level level, const char *fmt, ...) {
#ifdef DEBUG
        printf("DEBUG %s _level/level: %d/%d, pattern: [%s] (%s,%d)\n", __FUNCTION__, _level, level, _pattern.c_str(), __FILE__,__LINE__);
#endif

        log_level target_level =  this->level(); // we use level's accessor because access needs to be threadsafe

        if (target_level >= level) {

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

            auto syslog_level = level;

            if ( level == log_level::trace ){
                // The syslog_sink, considers that TRACE and DEBUG are the same.
                syslog_level = log_level::debug ;
            }

            std::string ecid = this->ecid(); // we use ecid's accessor because access needs to be threadsafe

            ::syslog ( syslog_level,
                    _pattern.c_str(),
                    ecid.empty() ? "" : ecid.c_str(),
                    buffer);
        }
    }

    void syslog_sink::set_name(const std::string &name) {
        sink::set_name(name);
        _pattern = "[L SUBSYS=" + name + "] %s %s";
    }

} // namespace logger
