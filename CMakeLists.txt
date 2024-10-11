cmake_minimum_required(VERSION 3.22)


project(eokas VERSION 1.0)


# Visual Studio 2022 doesn't support C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

if(WIN32)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

include("eokas.cmake")
include("eokas-base.cmake")
include("eokas-native.cmake")
include("eokas-datapot.cmake")
include("eokas-datapot-ui.cmake")
