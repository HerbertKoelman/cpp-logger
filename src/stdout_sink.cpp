//
//  Created by herbert koelman on 01/07/2016.
//

#include "logger/sinks.hpp"

namespace pmu {
  namespace log {
    
    stdout_sink::stdout_sink( const std::string &name, const std::string &pname, log_level level): file_sink(name, pname, level, stdout){
      // intentional...
    };

  } // namespace log
} // namespace pmu
