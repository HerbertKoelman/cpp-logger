message(STATUS "Loading SonarCloud module found here: ${CMAKE_CURRENT_LIST_DIR}")

option(SONAR "Generate sonar setup" ON)
set(SONAR_CLOUD_HOME  https://sonarcloud.io  CACHE STRING "Sonar cloud home URL(default is https://sonarcloud.io)")
set(SONAR_PROJECT_KEY        ${PROJECT_NAME} CACHE STRING "Sonar project key property (default is ${PROJECT_NAME})")
set(SONAR_ORGANIZATION                       CACHE STRING "Organization (default is empty)")
set(SONAR_ACCESS_TOKEN                       CACHE STRING "Access/login token (default is empty)")
set(SONAR_WRAPPER_OUTPUT_DIR  bw-output      CACHE STRING "Sonar C/C++ wrapper output directory (default is empty)")

set(SONAR_PROPERTIES_FILE                    CACHE STRING "If file exists, it will used as is (default is empty)")

if ( SONAR )

  find_program(SONAR_SCANNER sonar-scanner)

  if ( SONAR_SCANNER )
    if(NOT EXISTS ${SONAR_PROPERTIES_FILE} )
      message(STATUS "Generating SONAR properties file (${CMAKE_CURRENT_BINARY_DIR}/sonar-project.properties)")
      configure_file(${CMAKE_CURRENT_LIST_DIR}/sonar-project.properties.in sonar-project.properties)
      set(SONAR_PROPERTIES_FILE ./sonar-project.properties)
    else()
      message(STATUS "Sonar scanner property file was provided (${SONAR_PROPERTIES_FILE})")
    endif()

    add_custom_target( code-quality
      COMMAND ${SONAR_SCANNER} -Dproject.settings=${SONAR_PROPERTIES_FILE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "run ${SONAR_SCANNER} -Dproject.settings=${SONAR_PROPERTIES_FILE} "
      )
    message(STATUS "Added custom target [code-quality]...")

  else()
    message(SEND_ERROR "Failed to find the program [sonar_scanner], make sure sonar tools are installed.")
  endif()

else()
  message(STATUS "SONAR setup skipped, option was FALSE")
endif()

