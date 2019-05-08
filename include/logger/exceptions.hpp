//
// Created by Herbert Koelman on 2019-04-21.
//

#include <exception>
#include <string>

#ifndef CPP_LOGGER_EXCEPTIONS_HPP
#define CPP_LOGGER_EXCEPTIONS_HPP

namespace logger {


    /** \addtogroup logger_exceptions Exceptions
     * @{
     */

    /**
     * General purpose logger exception.
     *
     * @since 2.0.0
     */
    class logger_exception: public std::exception {
    public:
        /** @return an explanatory string */
        const char *what() const noexcept override;

        /** new logger exception.
         *
         * @param message an error message (default is "logger failed")
         */
        logger_exception( const std::string &message = "logger failed" );

        virtual ~logger_exception(){
            // intentional
        }

    private:

        std::string _message; //!< explanatory string
    };

    /** thrown by sinks.
     *
     * @since 2.0.0
     * @see logger::sink
     */
    class sink_exception: public logger_exception {
    public:

        /** new sink exception.
        *
        * @param message an error message (default is "sink failed")
        */
        sink_exception( const std::string &message = "sink failed" );

    };

    /** @} */
}
#endif //CPP_LOGGER_EXCEPTIONS_HPP
