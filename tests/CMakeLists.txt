# Load and compile GTest
# Aliases: GTest::GTest, GTest::gtest_main, GMock::GMock

add_executable(logger_tests logger_test.cpp)
target_link_libraries(logger_tests GTest::GTest cpp-logger-static GTest::gtest_main)

add_executable(logger_performance_tests logger_performance_tests.cpp)
target_link_libraries(logger_performance_tests GTest::GTest cpp-logger-static GTest::gtest_main)

add_test(NAME logger_tests COMMAND logger_tests)

