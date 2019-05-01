//
// Created by Herbert Koelman on 2019-04-14.
//

/* Unit tests to check that ficility implementatyions are working as exxpected
 */
#include <logger/cpp-logger.hpp>
#include "gtest/gtest.h"

TEST(syslog_facility, create_default) {

    logger::syslog::facility_ptr facility = logger::syslog::facility::default_facility();

    EXPECT_NE(facility, nullptr);
    EXPECT_EQ(facility->code(), 1);
    EXPECT_EQ(facility->keyword(), "user");
    EXPECT_EQ(facility->description(), "User-level messages");
}

TEST(syslog_facility, create_kern) {

	logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("kern");

	EXPECT_NE(facility, nullptr);
	EXPECT_EQ(facility->code(), 0);
	EXPECT_EQ(facility->keyword(), "kern");
	EXPECT_EQ(facility->description(), "Kernel messages");
}

TEST(syslog_facility, create_user) {

	logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("user");

	EXPECT_NE(facility, nullptr);
	EXPECT_EQ(facility->code(), 1);
	EXPECT_EQ(facility->keyword(), "user");
	EXPECT_EQ(facility->description(), "User-level messages");
}

TEST(syslog_facility, create_mail) {

	logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("mail");

	EXPECT_NE(facility, nullptr);
	EXPECT_EQ(facility->code(), 2);
	EXPECT_EQ(facility->keyword(), "mail");
	EXPECT_EQ(facility->description(), "Mail system");
}

TEST(syslog_facility, create_daemon) {

	logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("daemon");

	EXPECT_NE(facility, nullptr);
	EXPECT_EQ(facility->code(), 3);
	EXPECT_EQ(facility->keyword(), "daemon");
	EXPECT_EQ(facility->description(), "System daemons");
}

TEST(syslog_facility, create_auth) {

	logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("auth");

	EXPECT_NE(facility, nullptr);
	EXPECT_EQ(facility->code(), 4);
	EXPECT_EQ(facility->keyword(), "auth");
	EXPECT_EQ(facility->description(), "Security/authorization messages");
}

TEST(syslog_facility, create_local0) {

	logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("local0");

	EXPECT_NE(facility, nullptr);
	EXPECT_EQ(facility->code(), 16);
	EXPECT_EQ(facility->keyword(), "local0");
	EXPECT_EQ(facility->description(), "Local use (local0)");
}

TEST(syslog_facility, create_local1) {

	logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("local1");

	EXPECT_NE(facility, nullptr);
	EXPECT_EQ(facility->code(), 17);
	EXPECT_EQ(facility->keyword(), "local1");
	EXPECT_EQ(facility->description(), "Local use (local1)");
}

TEST(syslog_facility, create_unheard_of) {
    try {
        logger::syslog::facility_ptr facility = logger::syslog::facility::create_for("never heard of this one");
    } catch ( logger::logger_exception &err ){
        EXPECT_STREQ(err.what(), "[never heard of this one] is not a syslog key");
    }
}