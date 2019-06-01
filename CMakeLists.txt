# Name: C++ threading library
# Creation date: lundi 24 septembre 2018, 10:39:35 (UTC+0200)
# Description: C++ threading library that mimics the C++11 standard implementation.
#
cmake_minimum_required(VERSION 3.9)

project(
  cpp-logger
  VERSION 2.3.0
  DESCRIPTION "Simple C++ logger (${GIT_LOG})")

#find_package(Git CONFIG)
#find_package(Conan)

# main targets --------------------------------------------------
#
if( CMAKE_BUILD_TYPE MATCHES Release )
  if( GIT_LOG )
    add_definitions( -DCPP_LOGGER_VERSION="${PROJECT_VERSION} - ${GIT_LOG}")
  else()
    add_definitions( -DCPP_LOGGER_VERSION="${PROJECT_VERSION}")
  endif()
else()
  if( GIT_LOG )
    add_definitions( -DCPP_LOGGER_VERSION="${PROJECT_VERSION}-${GIT_LOG}-SNAPSHOT")
  else()
    add_definitions( -DCPP_LOGGER_VERSION="${PROJECT_VERSION}-SNAPSHOT")
  endif()
endif()

# configure_file(src/config.h.in ../include/logger/config.h)

# project's public headers
include_directories(include src)

set(LOGGER_SOURCE
        src/cpp-logger.cpp
        src/file_sink.cpp
        src/logger.cpp
        src/registry.cpp
        src/sink.cpp
        src/stderr_sink.cpp
        src/stdout_sink.cpp
        src/syslog_sink.cpp
        src/facilities.cpp include/logger/facilities.hpp
        src/exceptions.cpp include/logger/exceptions.hpp)

set(USED_COMPILER_FEATURES
        cxx_std_11
        )

# For future use
#include(WriteCompilerDetectionHeader)
#write_compiler_detection_header(
#        FILE "${CMAKE_CURRENT_BINARY_DIR}/logger_compiler_detection.h"
#        PREFIX LOGGER
#        COMPILERS GNU AppleClang Clang
#        PROLOG "// Requested features: ${USED_COMPILER_FEATURES}"
#        FEATURES
#        ${USED_COMPILER_FEATURES}
#        cxx_variadic_templates
#)

add_library            (cpp-logger-static STATIC ${LOGGER_SOURCE})
target_link_libraries  (cpp-logger-static pthread)
target_compile_features(cpp-logger-static PUBLIC ${USED_COMPILER_FEATURES})
set_target_properties  (cpp-logger-static PROPERTIES OUTPUT_NAME logger)

add_library            (cpp-logger-shared SHARED  ${LOGGER_SOURCE} )
target_link_libraries  (cpp-logger-shared pthread)
target_compile_features(cpp-logger-shared PUBLIC  ${USED_COMPILER_FEATURES})
set_target_properties  (cpp-logger-shared PROPERTIES OUTPUT_NAME logger)

# Testing -------------------------------------------------------
#
# Load and compile GTest
# Aliases: GTest::GTest, GTest::gtest_main, GMock::GMock
find_package(GTestExt PATHS cmake)
if (GTestExt_FOUND AND BUILD_TESTS)
  enable_testing()
  message(STATUS "Adding project's unit tests (in ./tests)...")
  add_subdirectory(tests)
endif()

# doxygen -------------------------------------------------------
#
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
if (Doxygen_FOUND)
  set(DOXYGEN_EXAMPLE_PATH tests)
  set(DOXYGEN_EXTRACT_ALL yes)
  set(DOXYGEN_PROJECT_BRIEF ${PROJECT_DESCRUPTION})
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
  doxygen_add_docs(doxygen README.md src include COMMENT "generate on-line documentation")
endif()

# install -------------------------------------------------------
#
install( TARGETS cpp-logger-static cpp-logger-shared DESTINATION lib )
install( DIRECTORY include DESTINATION include COMPONENT Devel)
install( DIRECTORY ${PROJECT_BINARY_DIR}/html/ DESTINATION doc/cpp-logger COMPONENT Documentation)

# CPACK ---------------------------------------------------------
#

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
# set(CPACK_GENERATOR "RPM")

set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Herbert Koelman")
set(CPACK_PACKAGE_VENDOR "Urbix Software")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
# set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)

cpack_add_install_type(develop DESCRIPTION "includes & libs (for developpers)")

# misc ------------------------------------------------------------
#
find_program(RM rm)
