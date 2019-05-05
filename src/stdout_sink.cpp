//
//  Created by herbert koelman on 01/07/2016.
//

#include "logger/sinks.hpp"

namespace logger {

    stdout_sink::stdout_sink(const std::string &name, const std::string &pname, log_level level) :
            file_sink(name, pname, level, stdout) {
        // intentional...
    }

    stdout_sink::stdout_sink() :
            file_sink("default", "pname", log_level::info, stdout){
        // intentional...
    };

} // namespace logger
