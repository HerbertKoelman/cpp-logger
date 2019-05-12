//
// Created by Herbert Koelman on 2019-04-14.
//

/* Unit tests to check that ficility implementatyions are working as exxpected
 */
#include <logger/cpp-logger.hpp>
#include "gtest/gtest.h"

TEST(facility, kern) {

    EXPECT_EQ(logger::syslog::kern_facility.code(), 0);
    EXPECT_STREQ(logger::syslog::kern_facility.keyword(), "kern");
    EXPECT_STREQ(logger::syslog::kern_facility.description(), "Kernel messages");

    std::cout << "facility: " << logger::syslog::kern_facility << std::endl;
}

TEST(facility, user) {

    EXPECT_EQ(logger::syslog::user_facility.code(), 1);
    EXPECT_STREQ(logger::syslog::user_facility.keyword(), "user");
    EXPECT_STREQ(logger::syslog::user_facility.description(), "User-level messages");

    std::cout << "facility: " << logger::syslog::user_facility << std::endl;
}

TEST(facility, mail) {

    EXPECT_EQ(logger::syslog::mail_facility.code(), 2);
    EXPECT_STREQ(logger::syslog::mail_facility.keyword(), "mail");
    EXPECT_STREQ(logger::syslog::mail_facility.description(), "Mail system");

    std::cout << "facility: " << logger::syslog::mail_facility << std::endl;
}

TEST(facility, daemon) {

    EXPECT_EQ(logger::syslog::daemon_facility.code(), 3);
    EXPECT_STREQ(logger::syslog::daemon_facility.keyword(), "daemon");
    EXPECT_STREQ(logger::syslog::daemon_facility.description(), "System daemons");

    std::cout << "facility: " << logger::syslog::daemon_facility << std::endl;
}

/*
constexpr facility user_facility{facility_code::auth,   "auth", "Security/authorization messages"};
constexpr facility user_facility{facility_code::local0, "local0", "Local use (local0)"};
constexpr facility user_facility{facility_code::local1, "local1", "Local use (local1)"};
*/

TEST(facility, auth) {

    EXPECT_EQ(logger::syslog::auth_facility.code(), 4);
    EXPECT_STREQ(logger::syslog::auth_facility.keyword(), "auth");
    EXPECT_STREQ(logger::syslog::auth_facility.description(), "Security/authorization messages");

    std::cout << "facility: " << logger::syslog::auth_facility << std::endl;
}

TEST(facility, local0) {

    EXPECT_EQ(logger::syslog::local0_facility.code(), 16);
    EXPECT_STREQ(logger::syslog::local0_facility.keyword(), "local0");
    EXPECT_STREQ(logger::syslog::local0_facility.description(), "Local use (local0)");

    std::cout << "facility: " << logger::syslog::local0_facility << std::endl;
}

TEST(facility, local1) {

    EXPECT_EQ(logger::syslog::local1_facility.code(), 17);
    EXPECT_STREQ(logger::syslog::local1_facility.keyword(), "local1");
    EXPECT_STREQ(logger::syslog::local1_facility.description(), "Local use (local1)");

    std::cout << "facility: " << logger::syslog::local0_facility << std::endl;
}
