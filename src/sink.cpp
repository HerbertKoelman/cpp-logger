//
//  logger.cpp
//  substation
//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include <pthread/pthread.hpp>
#include "logger/sinks.hpp"

namespace pmu {
  namespace log {
    
    // abstract sink class -------------------
    //
    sink::sink( const std::string &name, const std::string &pname, log_level level ): 
      // init class data
      _pid(getpid()), _name(name), _level(level), _pname(pname), _pattern(PMU_LOG_PATTERN)
    {

#ifdef DEBUG
      printf ("DEBUG %s pattern: [%s](%s,%d)\n", __FUNCTION__, _pattern.c_str(), __FILE__, __LINE__);
#endif

      set_facility(log_facility::sic_tux);
      _pattern = std::string(PMU_LOG_PATTERN) + "[L SUBSYS=" +_name + "] %s" ;
      _hostname[0] = 0;
      gethostname(_hostname, HOST_NAME_MAX);

      timeval current_time;
      gettimeofday(&current_time, NULL);

      struct std::tm local_time ;
      localtime_r(&current_time.tv_sec, &local_time);

      char buffer[10] ;// lag is in the form of "-02:00"
      memset (buffer, 0, 10 );

      int lag = (timezone/3600) * (-1 * ((local_time.tm_isdst == 1) ? 200:100));
      int hours = lag / 100 ;
      int minutes = lag - (hours * 100 );
      snprintf(buffer, 7, "%s%0.2d:%0.2d", 
          ((lag < 0) ? "-" : "+" ),
          hours,
          minutes
          );
      _lag = buffer ;
    }
    
    sink::~sink(){
      // TODO remove this printf("sink::~sink() destructor.\n");
    }
    
    std::string sink::ecid() {
      pthread::lock_guard<pthread::read_lock> lock(_ecid_rwlock);

      return _ecid;
    }

    void sink::set_ecid( const std::string &ecid ){

      pthread::lock_guard<pthread::write_lock> lock(_ecid_rwlock);

      if ( !ecid.empty() ) {
        _ecid = "[M ECID=\"" + ecid.substr(0, MAXECIDLEN) + "\"]";
      }
      else {
        _ecid = "- ";
      }
    }

    void sink::set_facility(log_facility facility){

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

    const std::string sink::date_time(){
      int  size = 50;
      char buffer[size];
      char target[size];

      memset(buffer,0,size);
      memset(target,0,size);

      timeval current_time;
      gettimeofday(&current_time, NULL);
      int micros = current_time.tv_usec ;

      struct std::tm local_time ;
      localtime_r(&current_time.tv_sec, &local_time);
      snprintf(target, size-1, "%d-%02d-%02dT%02d:%02d:%02d.%06d%s",
          local_time.tm_year+1900, // tm_year is the number of years from 1900
          local_time.tm_mon + 1,   // tm_mon is the month number starting from 0
          local_time.tm_mday,
          local_time.tm_hour,
          local_time.tm_min,
          local_time.tm_sec,
          micros,
          _lag.c_str());

      return std::string(target);
    }

  } // namespace log
} // namespace pmu
