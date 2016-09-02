/* created : Thu Jul  7 18:38:27 CEST 2016
 * author: herbert koelman (herbert.koelman@pmu.fr)
 */

#include <string>
#include <cstdio>
#include <pthread/pthread.hpp>
#include "logger/definitions.hpp"

#ifndef PMU_LOGGER_SINKS_HPP
#define PMU_LOGGER_SINKS_HPP

#include "logger/logger.hpp"

namespace pmu {
  namespace log {

    /** \addtogroup pmu_log_sinks Sinks
     * @{
     */

    /** default sink writes your stuff on stdout.
     *
     * @author herbert koelman(herbert.koelman@pmu.fr)
     * @since v1.3.0
     */
    class default_sink {
      public:

        /** default constructor
         */
        default_sink();

        /** log a message if current log level is >= level
         *
         * @param fmt formatting string (see printf for more informations)
         * @param args message arguments
         */
        template<typename... Args> void write( const log_level level, const std::string &fmt, const Args&... args){
          if ( _level >= level) {

            // we make a copy of the formatting string (fmt) because we receive a const string
            // and want to change things in i
            std::string format = fmt;

            printf(
                  // first, we build a format pattern made of the current pattern (which will hold the prefix and the fmt received
                  (
                    _pattern +
                    (format.at(format.size()-1) == '\n' ? format : format += '\n') // handle string termination
                  ).c_str(),
                _level,
                date_time().c_str(),
                _facility.c_str(),
                _pid,
                pthread::this_thread::get_id(),
                args...
            );
          }
        };

        /**
         * @param facility facility to use
         */
        void set_facility(log_facility facility);

      private:

        /** fill the buffer with the current date and time information
         */
        const std::string date_time();

        /** définit le préfix à utiliser
         *
         * @param pattern d.sir. (default "%s")
         */
        void set_pattern(std::string pattern);

        std::string  _pattern;
        std::string  _facility;
        pid_t        _pid;
        log_level    _level;
        pthread::mutex _mutex; //!< used to protect access to static class data
      };

    /** @} */

  } // namespace log
} // namespace pmu
#endif
