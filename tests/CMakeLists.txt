# Load and compile GTest
# Aliases: GTest::GTest, GTest::gtest_main, GMock::GMock

if (GCOV AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
  set(GCOV_LIB gcov)
endif()

add_executable(logger_tests logger_tests.cpp)
target_link_libraries(logger_tests GTest::GTest cpp-logger-static GTest::gtest_main ${GCOV_LIB})

add_executable(facility_tests facility_tests.cpp)
target_link_libraries(facility_tests GTest::GTest GMock::GMock cpp-logger-static GTest::gtest_main ${GCOV_LIB})

add_executable(exception_tests exception_tests.cpp)
target_link_libraries(exception_tests GTest::GTest GMock::GMock cpp-logger-static GTest::gtest_main ${GCOV_LIB})

add_executable(sink_tests sink_tests.cpp)
target_link_libraries(sink_tests GTest::GTest GMock::GMock cpp-logger-static GTest::gtest_main ${GCOV_LIB})

add_executable(logger_performance_tests logger_performance_tests.cpp)
target_link_libraries(logger_performance_tests GTest::GTest cpp-logger-static GTest::gtest_main ${GCOV_LIB})

add_test(NAME logger_tests   COMMAND logger_tests)
add_test(NAME facility_tests COMMAND facility_tests)
add_test(NAME exception_tests COMMAND exception_tests)
add_test(NAME sink_tests      COMMAND sink_tests)
