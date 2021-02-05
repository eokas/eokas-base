cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(libarchaism VERSION 1.0)

# specify the C++ standard and options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

message("project name: " ${PROJECT_NAME})
message("project source dir: " ${PROJECT_SOURCE_DIR})
message("project binary dir: " ${PROJECT_BINARY_DIR})   

aux_source_directory(./src SRC_DIR)
file(GLOB HEADER_FILES "./src/*.h")
message("headers " ${HEADER_FILES})

add_library(${PROJECT_NAME} STATIC ${SRC_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include)
