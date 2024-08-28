cmake_minimum_required(VERSION 3.22)

include("eokas.cmake")
include("eokas-android.cmake")


project(eokas VERSION 1.0)

eokas_android_setup_env()

# Visual Studio 2022 doesn't support C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")


include("eokas-base.cmake")
include("eokas-native.cmake")
