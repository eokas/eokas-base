cmake_minimum_required(VERSION 3.22)


project(eokas-base VERSION 1.0)


set(CMAKE_ANDROID_NDK D:\\Library\\Android\\AndroidNDK-RenderDoc\\21.4.7075529)

#32位
#set(CMAKE_C_COMPILER ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/bin/armv7a-linux-androideabi21-clang)
#set(CMAKE_CXX_COMPILER ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/bin/armv7a-linux-androideabi21-clang++)

#64位
set(CMAKE_BUILD_TOOL ${CMAKE_ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe)
set(CMAKE_C_COMPILER ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/bin/aarch64-linux-android21-clang.cmd)
set(CMAKE_CXX_COMPILER ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/bin/aarch64-linux-android21-clang++.cmd)
#add_definitions("--sysroot=/ndk/sysroot")

#https://cmake.org/cmake/help/v3.19/manual/cmake-toolchains.7.html#cross-compiling-for-android
set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 21) # API level
#set(CMAKE_ANDROID_ARCH_ABI armeabi-v7a)
set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)
#set(CMAKE_ANDROID_STL_TYPE c++_shared)
set(CMAKE_ANDROID_STL_TYPE c++_static)
set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)


# Visual Studio 2022 doesn't support C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


include("eokas.cmake")
include("eokas-base.cmake")
include("eokas-native.cmake")
