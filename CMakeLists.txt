cmake_minimum_required(VERSION 3.10)
project(libarchaism)

message(" project source dir: " ${PROJECT_SOURCE_DIR})
message(" project binary dir: " ${PROJECT_BINARY_DIR})   

aux_source_directory(./src SRC_DIR)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_library(${PROJECT_NAME} ${SRC_DIR})
# add_executable(${PROJECT_NAME}, ${SRC_DIR})

# target_link_libraries(${PROJECT_NAME} libcxx)
