//
//  stderr_sink.cpp
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "logger/sinks.hpp"

namespace logger {

    stderr_sink::stderr_sink(const std::string &name, const std::string &pname, log_level level) :
            file_sink(name, pname, level, stderr) {
        // intentional...
    }

    stderr_sink::stderr_sink() :
        file_sink("default", "pname", log_level::info, stderr){
    }

} // namespace logger
