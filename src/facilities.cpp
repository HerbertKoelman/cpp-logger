//
// Created by Herbert Koelman on 2019-04-14.
//

#include "logger/facilities.hpp"

namespace logger {

    // syslog facility -------------------------------------------------------------------------------------------------

    namespace syslog {

        std::ostream &operator<<(std::ostream &stream, const facility &facility) {
            stream << "facility: " << facility.keyword() << " - " << facility.description() << "(code: " << facility.code() << ")";
            return stream;
        }
    } // namespace syslog
} // namespace logger