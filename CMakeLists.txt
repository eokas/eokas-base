cmake_minimum_required(VERSION 3.22)


project(eokas VERSION 1.0)


# 检测是否支持 AVX2, SSE4.2 对于 x86/x86_64 架构
include(CheckCXXCompilerFlag)
include(CheckTypeSize)

if(MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)  # Check for 64-bit builds
        # On x64, MSVC defaults to SSE2, and you can enable AVX or AVX2 if needed.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
        message(STATUS "MSVC Compiler - Setting /arch:AVX2 flag")
    else()
        # For 32-bit builds, you might want to specify SSE2 or higher.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
        message(STATUS "MSVC Compiler - Setting /arch:SSE2 flag")
    endif()
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|x86)$")
        message(STATUS "This is AMD64: ${COMPILER_SUPPORTS_MARCH_NATIVE}")
        # 检查编译器是否支持 -march=native 选项，这将启用所有被本地机器支持的优化
        check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
        if(COMPILER_SUPPORTS_MARCH_NATIVE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
        endif()

        # 检查是否支持 AVX2
        check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
        if(COMPILER_SUPPORTS_AVX2)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
        endif()

        # 检查是否支持 SSE4.2
        check_cxx_compiler_flag("-msse4.2" COMPILER_SUPPORTS_SSE4_2)
        if(COMPILER_SUPPORTS_SSE4_2)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
        endif()
    endif()

    # 检测是否支持 NEON 对于 ARM 架构
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)$")
        # 检查编译器是否支持 NEON
        check_cxx_compiler_flag("-mfpu=neon" COMPILER_SUPPORTS_NEON)
        if(COMPILER_SUPPORTS_NEON)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")
        endif()
    endif()
endif()

# 打印最终的编译选项
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")


# Visual Studio 2022 doesn't support C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

include("eokas.cmake")
include("eokas-base.cmake")
include("eokas-native.cmake")
include("eokas-gpu.cmake")
include("eokas-datapot.cmake")
include("eokas-app.cmake")
include("eokas-datapot-ui.cmake")
include("eokas-test-gpu.cmake")
