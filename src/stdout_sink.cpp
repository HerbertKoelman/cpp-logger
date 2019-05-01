//
//  Created by herbert koelman on 01/07/2016.
//

#include "logger/sinks.hpp"

namespace logger {

    stdout_sink::stdout_sink(const std::string &name, const std::string &pname, log_level level, const log_facility_ptr &facility) :
            file_sink(name, pname, level, facility, stdout) {
        // intentional...
    };


    stdout_sink::stdout_sink(const std::string &name, const std::string &pname, log_level level):
            file_sink(name, pname, level, syslog::facility::default_facility(), stdout){

    }

} // namespace logger
