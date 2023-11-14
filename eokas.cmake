
cmake_host_system_information(RESULT EOKAS_OS_NAME QUERY OS_NAME)
message("EOKAS_OS_NNAME = " ${EOKAS_OS_NAME})

set(EOKAS_PROJECT_NAME "${PROJECT_NAME}")
set(EOKAS_PROJECT_DIR "${PROJECT_SOURCE_DIR}")
set(EOKAS_BINARY_DIR "${PROJECT_BINARY_DIR}")
set(EOKAS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

message("EOKAS_PROJECT_NAME = " ${EOKAS_PROJECT_NAME})
message("EOKAS_PROJECT_DIR = " ${EOKAS_PROJECT_DIR})
message("EOKAS_SOURCE_DIR = " ${EOKAS_SOURCE_DIR})
message("EOKAS_BINARY_DIR = " ${EOKAS_BINARY_DIR})

macro(eokas_test_setup TEST_LIB_NAME)
    message("Test: Start...")
    if(${CMAKE_PROJECT_NAME} STREQUAL ${TEST_LIB_NAME})
        message("Test: " ${TEST_LIB_NAME})
        include(CTest)

        if(${BUILD_TESTING})
            set(TEST_EXEC_NAME ${TEST_LIB_NAME}-test)
            message("Test: Build " ${TEST_EXEC_NAME})

            file(GLOB TEST_ENGINE_FILES ./test/engine/*.cpp)
            file(GLOB TEST_CASES_FILES ./test/cases/*.cpp)
            add_executable(${TEST_EXEC_NAME} ${TEST_ENGINE_FILES} ${TEST_CASES_FILES})
            target_link_libraries(${TEST_EXEC_NAME} PRIVATE ${TEST_LIB_NAME})

            foreach (FILE_NAME ${TEST_CASES_FILES})
                get_filename_component (TEST_CASE_NAME ${FILE_NAME} NAME_WE)
                add_test (NAME ${TEST_CASE_NAME} COMMAND ${TEST_EXEC_NAME} ${TEST_CASE_NAME})
                message("Test: Case> " ${TEST_CASE_NAME} ${FILE_NAME})
            endforeach ()
        endif()
    endif()
endmacro()
