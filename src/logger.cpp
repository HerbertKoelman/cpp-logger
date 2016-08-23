//
//  logger.cpp
//  substation
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include <pthread/pthread.hpp>
#include "logger/logger.hpp"

namespace pmu {
  namespace log {
    
    logger::logger( const std::string &name, const std::string &pname, log_level level ): _pid(getpid()), _name(name), _pname(pname), _pattern(PMU_LOG_PATTERN), _level(level){
      set_facility(log_facility::sic_tux);
      _pattern = std::string(PMU_LOG_PATTERN) + "[" +_name + "] " ;
      _hostname[0] = 0;
      gethostname(_hostname, HOST_NAME_MAX);
    }
    
    logger::~logger(){
      // TODO remove this printf("logger::~logger() destructor.\n");
    }
    
    void logger::set_facility(log_facility facility){
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

    void logger::set_name(const std::string &name){
      pthread::lock_guard<pthread::mutex> lock(_mutex);
      _name = name ;
      _pattern = std::string(PMU_LOG_PATTERN) + _name + ": " ;
    };

    void logger::set_pattern(const std::string pattern){
      _pattern = pattern ;
    };

    const std::string logger::date_time(){
      int  size = 100;
      char buffer[size];
      char target[size];

      memset(buffer,0,size);
      memset(target,0,size);

      timeval current_time;
      gettimeofday(&current_time, NULL);
      //int millis = current_time.tv_usec / 1000;
      int micros = current_time.tv_usec ;

      strftime(buffer, size-1, "%FT%T", localtime(&current_time.tv_sec));

      snprintf(target, size-1, "%s.%06d%+05d", buffer, micros,(timezone/3600) * -100);

      return std::string(target);
    }

  } // namespace log
} // namespace pmu
