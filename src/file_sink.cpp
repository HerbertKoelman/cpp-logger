//
//  Created by herbert koelman on 01/07/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include "logger/sinks.hpp"

namespace pmu {
  namespace log {

    file_sink::file_sink( const std::string &name, const std::string &pname, log_level level, FILE *file): sink(name, pname, level), _file_descriptor(file){
      // intentional...
    };

    file_sink::~file_sink(){
      //fflush(_file_descriptor);
    };

    void file_sink::write( log_level level, const char *fmt, ... ){
#ifdef DEBUG
      printf("DEBUG %s _level/level: %d/%d, pattern: [%s] (%s,%d)\n", __FUNCTION__, _level, level, _pattern.c_str(), __FILE__,__LINE__);
#endif

      if ( _level >= level) {

        // fill a buffer with the user message
        va_list args1;
        va_start(args1, fmt );
        va_list args2;
        va_copy(args2, args1); // save a copy

        // new character vector
        // vsnprintf returns the needed characters to store the outpu
        // not possible with XXL C/C++ std::vector<char> buf(1+std::vsnprintf(NULL, 0, fmt, args1));

        size_t buffer_size = 1+vsnprintf(NULL, 0, fmt, args1);
        va_end(args1); // don't need this one anymore

        char buffer[buffer_size];
        memset(buffer,0,buffer_size);

        // fill buffer with message ...
        vsnprintf(buffer, buffer_size, fmt, args2);
        va_end(args2);

#ifdef DEBUG
        printf ("DEBUG pattern: [%s], file: %d\nDEBUG found end-of-line character in [%s]: %s (%s,%d)\n",
            _pattern.c_str(),
            _file_descriptor,
            buffer,
            buffer[buffer_size-2] == '\n' ? "yes" : "no",
            __FILE__,
            __LINE__);
#endif
        // add a new line if not already there
        if ( buffer[buffer_size-2] != '\n' ){
          buffer[buffer_size-1] = '\n';
        }

        fprintf(
            _file_descriptor,
            _pattern.c_str(),
            level,
            date_time().c_str(),
            _facility.c_str(),
            _hostname,
            _pname.c_str(),
            _pid,
            pthread::this_thread::get_id(),
            _ecid.empty()? "- " : _ecid.c_str(),
            buffer
        );
      }
    }; // write
  } // namespace log
} // namespace pmu
