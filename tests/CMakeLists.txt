

enable_testing()

add_executable(logger_tests logger_test.cpp)
target_link_libraries(logger_tests GTest::GTest cpp-logger-static GTest::gtest_main)
add_test(NAME logger_tests COMMAND logger_tests)
