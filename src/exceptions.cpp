//
// Created by Herbert Koelman on 2019-04-21.
//

#include "logger/exceptions.hpp"

namespace logger {

    logger_exception::logger_exception( const std::string &message ): _message(message){
        // intentional
    }

    const char *logger_exception::what() const noexcept {
        return _message.c_str();
    }

    sink_exception::sink_exception(const std::string &message) : logger_exception(message) {
        // intentional
    }

    facility_exception::facility_exception(const std::string &message) : logger_exception(message) {
       // intentional
    }
}