cmake_minimum_required(VERSION 3.0)

project(libarchaism)

aux_source_directory(./src, SRC_DIR)

# add_subdirectory(.)

add_library(libarchaism, ${SRC_DIR})
# add_executable(libarchaism, ${SRC_DIR})

# target_link_libraries(libarchaism libcxx)
