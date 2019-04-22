//
// Created by Herbert Koelman on 2019-04-21.
//

#include <logger/cpp-logger.hpp>
#include "gtest/gtest.h"

TEST(exceptions, logger_exception) {
    try {
        throw logger::logger_exception() ;
    } catch ( std::exception &err ) {
        EXPECT_STREQ(err.what(), "logger failed");
    }
}

TEST(exceptions, logger_exception_message) {
    try {
        throw logger::logger_exception("sink failed to initialize") ;
    } catch ( std::exception &err ) {
        EXPECT_STREQ(err.what(), "sink failed to initialize");
    }
}

TEST(exceptions, sink_exception) {
    try {
        throw logger::sink_exception() ;
    } catch ( std::exception &err ) {
        EXPECT_STREQ(err.what(), "sink failed");
    }
}

TEST(exceptions, facility_exception) {
    try {
        throw logger::facility_exception() ;
    } catch ( std::exception &err ) {
        EXPECT_STREQ(err.what(), "facility failed");
    }
}