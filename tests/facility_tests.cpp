//
// Created by Herbert Koelman on 2019-04-14.
//

/* Unit tests to check that ficility implementatyions are working as exxpected
 */
#include <logger/cpp-logger.hpp>
#include "gtest/gtest.h"

TEST(syslog_facility, create_default) {

    logger::syslog_facility_ptr facility = logger::syslog_facility::default_facility();

    EXPECT_NE(facility, nullptr);
    EXPECT_EQ(facility->code(), 1);
    EXPECT_EQ(facility->keyword(), "user");
    EXPECT_EQ(facility->description(), "User-level messages");
}

TEST(syslog_facility, create_kern) {

    logger::syslog_facility_ptr facility = logger::syslog_facility::kern_facility();

    EXPECT_NE(facility, nullptr);
    EXPECT_EQ(facility->code(), 0);
    EXPECT_EQ(facility->keyword(), "kern");
    EXPECT_EQ(facility->description(), "Kernel messages");
}