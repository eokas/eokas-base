cmake_minimum_required(VERSION 3.22)


project(eokas VERSION 1.0)


# Visual Studio 2022 doesn't support C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")


include("cmake/eokas.cmake")
include("cmake/eokas-base.cmake")
include("cmake/eokas-native.cmake")
include("cmake/eokas-gpu.cmake")
include("cmake/eokas-rose.cmake")
include("cmake/eokas-app.cmake")
include("cmake/eokas-rose-ui.cmake")
include("cmake/eokas-test-gpu.cmake")
