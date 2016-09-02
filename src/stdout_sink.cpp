//
//  default_sink.cpp
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "logger/sinks.hpp"

namespace pmu {
  namespace log {

    default_sink::default_sink(): _pid(getpid()), _pattern(PMU_LOG_PATTERN){
      // intentional...
    }

    const std::string default_sink::date_time(){
      int  size = 100;
      char buffer[size];
      char target[size];

      memset(buffer,0,size);
      memset(target,0,size);

      timeval current_time;
      gettimeofday(&current_time, NULL);
      int millis = current_time.tv_usec / 1000;

      strftime(buffer, size-1, "%d/%m/%Y %Z %T", localtime(&current_time.tv_sec));

      snprintf(target, size-1, "%s.%d", buffer, millis);

      return std::string(target);
    }

    void default_sink::set_pattern(const std::string pattern){
      _pattern = pattern ;
    };

    void default_sink::set_facility(log_facility facility){
      pthread::lock_guard<pthread::mutex> lock(_mutex);

      switch(facility) {
        case sic_bat:
          _facility="SIC/BAT";
          break;
        case sic_ine:
          _facility="SIC/INE";
          break;
        case sic_bmp:
          _facility="SIC/BMP";
          break;
        case sic_mpp:
          _facility="SIC/MPP";
          break;
        case sic_kix:
          _facility="SIC/KIX";
          break;
        case sic_tux:
        default:
          _facility="SIC/TUX";
      }
    }
  } // namespace log
} // namespace pmu
