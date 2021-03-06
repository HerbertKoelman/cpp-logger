//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include <sys/time.h>
#include "logger/sinks.hpp"
#include <cstring>

namespace logger {


    file_sink::file_sink(FILE *file) :
      file_sink("file-sink", "app", log_level::info, file){
    }

    file_sink::file_sink(const std::string &name, const std::string &pname, log_level level, FILE *file) :
            sink(name, pname, level),
            _file_descriptor(file),
            _pid(getpid()) {
#ifdef DEBUG
        printf ("DEBUG %s pattern: [%s](%s,%d)\n", __FUNCTION__, _pattern.c_str(), __FILE__, __LINE__);
#endif

        // this will set sink's name and set/reset the static part of the messages this sink will produce.
        set_name(name);

        char hostname[HOST_NAME_MAX];
        hostname[0] = 0;
        gethostname(hostname, HOST_NAME_MAX);
        _hostname = hostname;

        timeval current_time{0};
        gettimeofday(&current_time, nullptr);

        struct std::tm local_time{0};
        localtime_r(&current_time.tv_sec, &local_time);

        char buffer[10];// lag is in the form of "-02:00"
        memset(buffer, 0, 10);

        int lag = (timezone / 3600) * (-1 * ((local_time.tm_isdst == 1) ? 200 : 100));
        int hours = lag / 100;
        int minutes = lag - (hours * 100);
        snprintf(buffer, 7, "%s%0.2d:%0.2d",
                 ((lag < 0) ? "-" : "+"),
                 hours,
                 minutes
        );
        _lag = buffer;
    };

    void file_sink::set_name(const std::string &name) {
        sink::set_name(name);

        _pattern = std::string(LOGGER_LOG_PATTERN) + "[L SUBSYS=" + name + "] %s";
    }

    void file_sink::write(log_level level, const char *fmt, ...) {
#ifdef DEBUG
        printf("DEBUG %s _level/level: %d/%d, level name: %s, pattern: [%s] (%s,%d)\n",
            __FUNCTION__,
            level(),
            level,
            log_level_name(level).c_str(),
            _pattern.c_str(),
            __FILE__,__LINE__);
#endif

        log_level target_level =  this->level(); // we use level's accessor because access needs to be threadsafe

        if (target_level >= level) {

            // fill a buffer with the user message
            va_list args1;
            va_list args2;

            va_start(args1, fmt); // initialize va_list
            va_copy(args2, args1); // save a copy args1 -> args2

            // new character vector
            // vsnprintf returns the needed characters to store the output
            //NOSONAR not possible with XL C/C++ std::vector<char> buf(1+std::vsnprintf(NULL, 0, fmt, args1));

            // this call only returns the actual number of bytes needed to store the message.
            // It doesn't count the needed end-of-string
            size_t buffer_size = vsnprintf(nullptr, 0, fmt, args1) + 1; // plus 1 character to store \0
            va_end(args1); // don't need this one anymore

            // add one more character to handle \n (see code below)
            char buffer[buffer_size + 1];
            memset(buffer, 0, buffer_size + 1);

            // fill buffer with message ...
            vsnprintf(buffer, buffer_size, fmt, args2);
            size_t len = strlen(buffer);
            va_end(args2);

#ifdef DEBUG
            printf ("DEBUG pattern: [%s], file: %d\nDEBUG found end-of-line character in [%s]: %s (%s,%d)\n",
                _pattern.c_str(),
                _file_descriptor,
                buffer,
                buffer[len-1] == '\n' ? "yes" : "no",
                __FILE__,
                __LINE__);
#endif
            // add a new line if not already there
            if (buffer[len - 1] != '\n') {
                buffer[len] = '\n'; // override ending \0 (that's why we provisioned for one more character above)
            }

            std::string ecid = this->ecid(); // we use ecid's accessor because access needs to be threadsafe

            fprintf(
                    _file_descriptor,
                    _pattern.c_str(),
                    target_level,
                    date_time().c_str(),
                    _hostname.c_str(),
                    program_name().c_str(),
                    _pid,
                    std::this_thread::get_id(),
                    ecid.empty() ? "- " : ecid.c_str(),
                    buffer
            );
        }
    }; // write

    const std::string file_sink::date_time() {
        int size = 50;
        char buffer[size];
        char target[size];

        memset(buffer, 0, size);
        memset(target, 0, size);

        timeval current_time{0};
        gettimeofday(&current_time, nullptr);
        int micros = current_time.tv_usec;

        struct std::tm local_time{0};
        localtime_r(&current_time.tv_sec, &local_time);
        snprintf(target, size - 1, "%d-%02d-%02dT%02d:%02d:%02d.%06d%s",
                 local_time.tm_year + 1900, // tm_year is the number of years from 1900
                 local_time.tm_mon + 1,   // tm_mon is the month number starting from 0
                 local_time.tm_mday,
                 local_time.tm_hour,
                 local_time.tm_min,
                 local_time.tm_sec,
                 micros,
                 _lag.c_str());

        return std::string(target);
    }

} // namespace logger
