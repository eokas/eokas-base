cmake_minimum_required(VERSION 3.22)

## Get system info
cmake_host_system_information(RESULT OS_NAME QUERY OS_NAME)
message("OS_NNAME = " ${OS_NAME})

## Project name and version
project(eokas-base VERSION 1.0)

## C++ Standard and Options
# Visual Studio 2022 doesn't support C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


## Project Information
message("project name: " ${PROJECT_NAME})
message("project source dir: " ${PROJECT_SOURCE_DIR})
message("project binary dir: " ${PROJECT_BINARY_DIR})   


## Main Library Target
aux_source_directory(./src SRC_DIR)
add_library(${PROJECT_NAME} STATIC ${SRC_DIR})


## Install
file(GLOB HEADER_FILES "./src/*.h")
message("headers " ${HEADER_FILES})
install(FILES ${HEADER_FILES} DESTINATION include/${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${OS_NAME}/${CMAKE_BUILD_TYPE})


## Test
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if(BUILD_TESTING)
        set(TEST_EXEC_NAME ${PROJECT_NAME}-test)
        set(TEST_CASE_PREFIX ${PROJECT_NAME})

        aux_source_directory(./test/engine TEST_ENGINE_DIR)
        aux_source_directory(./test/cases TEST_CASES_DIR)
        add_executable(${TEST_EXEC_NAME} ${TEST_ENGINE_DIR} ${TEST_CASES_DIR})
        target_link_libraries(${TEST_EXEC_NAME} PRIVATE ${PROJECT_NAME})

        foreach (FILE_NAME ${TEST_CASES_DIR})
            get_filename_component (TEST_CASE_NAME ${FILE_NAME} NAME_WE)
            add_test (NAME ${TEST_CASE_NAME} COMMAND ${TEST_EXEC_NAME} ${TEST_CASE_NAME})
        endforeach ()
    endif()
endif()
