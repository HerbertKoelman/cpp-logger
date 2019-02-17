# Testing -------------------------------------------------------
#

# Name: Test-Herbert
# Creation date: mercredi 7 novembre 2018, 10:13:42 (UTC+0100)
# Description: C++ logger library test
#

# Locate GTest
find_package(GTest)
if (GTest_FOUND)

  enable_testing()

  add_executable(logger_tests logger_test.cpp)
  target_link_libraries(logger_tests GTest::GTest cpp-logger-static gtest_main)
  add_test(NAME logger_tests COMMAND logger_tests)

endif()
