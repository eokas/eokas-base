

function(eokas_android_check_jdk)
    if(NOT DEFINED ENV{JAVA_HOME})
        message(FATAL_ERROR "JAVA_HOME environment variable must be defined for Android build")
    endif()
    message(STATUS "Using JAVA_HOME = $ENV{JAVA_HOME}")
    execute_process(COMMAND $ENV{JAVA_HOME}/bin/java -version
            RESULT_VARIABLE _result
            OUTPUT_VARIABLE _output
            ERROR_VARIABLE _output
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)
    if(NOT _result AND _output MATCHES "version \"([0-9]+).([0-9]+)")
        message(STATUS "Java in JAVA_HOME is ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}")
    else()
        message(STATUS "Java in JAVA_HOME is unknown version ${_output} ${_result}")
    endif()
endfunction()


function(eokas_android_check_sdk)
    if(DEFINED ENV{ANDROID_HOME} AND EXISTS "$ENV{ANDROID_HOME}/build-tools")
        set(ANDROID_SDK_ROOT_PATH "$ENV{ANDROID_HOME}")
    elseif(DEFINED ENV{ANDROID_SDK_ROOT} AND EXISTS "$ENV{ANDROID_SDK_ROOT}/build-tools")
        set(ANDROID_SDK_ROOT_PATH "$ENV{ANDROID_SDK_ROOT}")
    elseif(DEFINED ENV{ANDROID_SDK} AND EXISTS "$ENV{ANDROID_SDK}/build-tools")
        set(ANDROID_SDK_ROOT_PATH "$ENV{ANDROID_SDK}")
    else()
        message(FATAL_ERROR "Can't locate Android SDK, set ANDROID_HOME, ANDROID_SDK_ROOT or ANDROID_SDK")
    endif()
    message(STATUS "Using Android SDK found in ${ANDROID_SDK_ROOT_PATH}")
endfunction()


function(eokas_android_check_ndk)
    if(DEFINED ENV{ANDROID_NDK_HOME} AND EXISTS "$ENV{ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake")
        set(ANDROID_NDK_ROOT_PATH "$ENV{ANDROID_NDK_HOME}")
    elseif(DEFINED ENV{ANDROID_NDK_ROOT} AND EXISTS "$ENV{ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake")
        set(ANDROID_NDK_ROOT_PATH "$ENV{ANDROID_NDK_ROOT}")
    elseif(DEFINED ENV{NDK_HOME} AND EXISTS "$ENV{NDK_HOME}/build/cmake/android.toolchain.cmake")
        set(ANDROID_NDK_ROOT_PATH "$ENV{NDK_HOME}")
    elseif(DEFINED ENV{ANDROID_NDK} AND EXISTS "$ENV{ANDROID_NDK}/build/cmake/android.toolchain.cmake")
        set(ANDROID_NDK_ROOT_PATH "$ENV{ANDROID_NDK}")
    else()
        message(FATAL_ERROR "Can't locate Android NDK, set ANDROID_NDK_HOME, ANDROID_NDK_ROOT, NDK_HOME or ANDROID_NDK")
    endif()
    message(STATUS "Using Android NDK found in ${ANDROID_NDK_ROOT_PATH}")
endfunction()


# Configure some stuff that needs to be set really early
function(eokas_android_setup_env)
    eokas_android_check_jdk()
    eokas_android_check_sdk()
    eokas_android_check_ndk()

    set(CMAKE_TOOLCHAIN_FILE
        "${ANDROID_NDK_ROOT_PATH}/build/cmake/android.toolchain.cmake"
        CACHE STRING "The Android toolchain file")

    # Choose clang if the NDK has both gcc and clang, since gcc sometimes fails
    set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION "clang")

    # AndroidToolChainOptions
    # Set default API level to 21 if not configured explicitly
    if(NOT ANDROID_PLATFORM)
        set(ANDROID_PLATFORM "android-21")
    endif()

    # AndroidToolChainOptions
    # Default to libc++_static as the other options can cause crashes
    if(NOT ANDROID_STL)
        set(ANDROID_STL "c++_static")
    endif()

    # AndroidToolChainOptions
    # Default to arm32 if nothing is specified on the command line.
    # Options are {armeabi-v7a,arm64-v8a}
    set(ANDROID_ABI "armeabi-v7a" CACHE STRING "The Android ABI to build for")

    # Setup eokas android options
    option(EOKAS_ANDROID_STRIP_LIBRARY "Strip the resulting android library" OFF)
endfunction()


function(eokas_android_setup_apk
        EOKAS_TARGET_NAME
        EOKAS_APK_PACKAGE_NAME
        EOKAS_APK_JAVA_SRC_DIR
        EOKAS_APK_VERSION_MAJOR
        EOKAS_APK_VERSION_MINOR
        EOKAS_APK_DEPENDS_TARGETS
)
    if(ANDROID)
        # Android sets this to off becuase Android is always terrible forever.
        # It breaks finding java in the path, so enable it again
        set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH ON)

        #############################
        # We need to check that 'java' in PATH is new enough. Temporarily unset the JAVA_HOME env,
        # then invoke FindJava.cmake which will search just the PATH, then re-set it.
        set(SAVE_JAVA_HOME $ENV{JAVA_HOME})
        set(ENV{JAVA_HOME} "")
        find_package(Java)
        set(ENV{JAVA_HOME} ${SAVE_JAVA_HOME})
        if(NOT ${Java_FOUND})
            message(FATAL_ERROR "Building Android requires the 'java' program in your PATH. It must be at least Java 8 (1.8)")
        endif()

        if(${Java_VERSION} VERSION_LESS 1.8)
            message(FATAL_ERROR "Building Android requires the 'java' program in your PATH to be at least Java 8 (1.8)")
        endif()
        message(STATUS "Using Java of version ${Java_VERSION}")

        if(EOKAS_ANDROID_STRIP_LIBRARY AND ANDROID_STRIP_TOOL AND RELEASE_MODE)
            add_custom_command(TARGET ${EOKAS_TARGET_NAME} POST_BUILD
                    COMMAND echo Stripping $<TARGET_FILE:${EOKAS_TARGET_NAME}>
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EOKAS_TARGET_NAME}> $<TARGET_FILE:${EOKAS_TARGET_NAME}>.dbg
                    COMMAND ${ANDROID_STRIP_TOOL} --strip-unneeded $<TARGET_FILE:${EOKAS_TARGET_NAME}>)
        endif()

        set(ANDROID_BUILD_TOOLS_VERSION "" CACHE STRING "Version of Android build-tools to use instead of the default")
        if(ANDROID_BUILD_TOOLS_VERSION STREQUAL "")
            # Enumerate the build tools versions available, and pick the most recent
            file(GLOB __buildTools RELATIVE "${ANDROID_SDK_ROOT_PATH}/build-tools" "${ANDROID_SDK_ROOT_PATH}/build-tools/*")
            list(SORT __buildTools)
            list(GET __buildTools -1 ANDROID_BUILD_TOOLS_VERSION)
            unset(__buildTools)
        endif()
        message(STATUS "Using Android build-tools version ${ANDROID_BUILD_TOOLS_VERSION}")

        set(APK_TARGET_ID "" CACHE STRING "The Target ID to build the APK for like 'android-99', use <android list targets> to choose another one.")
        if(APK_TARGET_ID STREQUAL "")
            # This seems different from the platform we're targetting,
            # default to the latest available that's greater or equal to our target platform
            file(GLOB __platforms RELATIVE "${ANDROID_SDK_ROOT_PATH}/platforms" "${ANDROID_SDK_ROOT_PATH}/platforms/*")
            list(SORT __platforms)
            # In case we don't find one, target the latest platform
            list(GET __platforms -1 APK_TARGET_ID)
            string(REPLACE "android-" "" __targetPlat "${ANDROID_PLATFORM}")

            # We require at least android 23 for Activity.requestPermissions
            if(__targetPlat LESS 23)
                set(__targetPlat 23)
            endif()

            foreach( __plat ${__platforms})
                string(REPLACE "android-" "" __curPlat "${__plat}")

                if(NOT (__curPlat LESS __targetPlat) )
                    set(APK_TARGET_ID "android-${__curPlat}")
                    break()
                endif()
            endforeach()

            unset(__platforms)
            unset(__targetPlat)
            unset(__curPlat)
        endif()
        message(STATUS "Using android.jar from platform ${APK_TARGET_ID}")

        # Suffix for scripts rather than binaries, which is needed explicitly on windows
        set(TOOL_SCRIPT_EXTENSION "")
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
            set(TOOL_SCRIPT_EXTENSION ".bat")
        endif()

        set(BUILD_TOOLS "${ANDROID_SDK_ROOT_PATH}/build-tools/${ANDROID_BUILD_TOOLS_VERSION}")
        set(RT_JAR "$ENV{JAVA_HOME}/jre/lib/rt.jar")
        set(JAVA_BIN "$ENV{JAVA_HOME}/bin")

        string(REPLACE "\\" "/" ANDROID_JAR "${ANDROID_SDK_ROOT_PATH}/platforms/${APK_TARGET_ID}/android.jar")
        if(CMAKE_HOST_WIN32)
            set(CLASS_PATH "${ANDROID_JAR}\;obj")
        else()
            set(CLASS_PATH "${ANDROID_JAR}:obj")
        endif()
        set(KEYSTORE ${CMAKE_CURRENT_BINARY_DIR}/debug.keystore)
        add_custom_command(OUTPUT ${KEYSTORE}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMAND ${JAVA_BIN}/keytool -genkey -keystore ${KEYSTORE} -storepass android -alias rdocandroidkey -keypass android -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=, OU=, O=, L=, S=, C=")

        # APK_VERSION_CODE corresponds to android:versionCode, an internal integer value that can be queried. Higher numbers indicate more recent versions.
        # APK_VERSION_NAME corresponds to android:versionName, a string value that is displayed to the user.
        set(APK_VERSION_CODE "${EOKAS_APK_VERSION_MAJOR}${EOKAS_APK_VERSION_MINOR}")
        set(APK_VERSION_NAME ${GIT_COMMIT_HASH})
        message(STATUS "Building APK versionCode ${APK_VERSION_CODE}, versionName ${APK_VERSION_NAME}")

        # Set the package name based on the ABI
        if(ANDROID_ABI STREQUAL "armeabi-v7a")
            set(ABI_EXTENSION_NAME "arm32")
        elseif(ANDROID_ABI STREQUAL "arm64-v8a")
            set(ABI_EXTENSION_NAME "arm64")
        elseif(ANDROID_ABI STREQUAL "x86")
            set(ABI_EXTENSION_NAME "x86")
        elseif(ANDROID_ABI STREQUAL "x86_64")
            set(ABI_EXTENSION_NAME "x64")
        else()
            message(FATAL_ERROR "ABI ${ANDROID_ABI} is not supported.")
        endif()

        set(EOKAS_APK_NAME "${EOKAS_APK_PACKAGE_NAME}.${ABI_EXTENSION_NAME}")
        set(EOKAS_APK_FILE ${CMAKE_BINARY_DIR}/bin/${EOKAS_APK_NAME}.apk)
        add_custom_target(apk ALL DEPENDS ${EOKAS_APK_FILE})

        option(ENABLE_CUSTOM_WRAP_SCRIPT "Enable custom wrap.sh on Android to workaround Android bug" ON)

        # Copy in android package files, replacing the package name with the architecture-specific package name
        configure_file(android/Loader.java ${CMAKE_CURRENT_BINARY_DIR}/src/${EOKAS_APK_JAVA_SRC_DIR}/Loader.java)
        configure_file(android/AndroidManifest.xml ${CMAKE_CURRENT_BINARY_DIR}/AndroidManifest.xml)
        configure_file(android/icon.png ${CMAKE_CURRENT_BINARY_DIR}/res/drawable/icon.png COPYONLY)

        add_custom_command(OUTPUT ${EOKAS_APK_FILE}
                DEPENDS ${TARGET_NAME}
                DEPENDS ${KEYSTORE}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMAND ${CMAKE_COMMAND} -E make_directory libs/lib/${ANDROID_ABI}
                COMMAND ${CMAKE_COMMAND} -E make_directory obj
                COMMAND ${CMAKE_COMMAND} -E make_directory bin
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}> libs/lib/${ANDROID_ABI}/$<TARGET_FILE_NAME:${TARGET_NAME}>
        )
        foreach(__TARGET ${EOKAS_APK_DEPENDS_TARGETS})
            add_custom_command(OUTPUT ${EOKAS_APK_FILE}
                    DEPENDS ${__TARGET}
                    DEPENDS ${TARGET_NAME}
                    DEPENDS ${KEYSTORE}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${__TARGET}> libs/lib/${ANDROID_ABI}/$<TARGET_FILE_NAME:${__TARGET}>
            )
        endforeach()

        if(ENABLE_CUSTOM_WRAP_SCRIPT)
            # use configure_file to ensure unix newlines regardless of how it is in the repository (e.g. CRLF on windows)
            configure_file(android/wrap.sh ${CMAKE_CURRENT_BINARY_DIR}/libs/lib/${ANDROID_ABI}/wrap.sh @ONLY NEWLINE_STYLE UNIX)

            message(STATUS "Using custom wrap script for Android bug workaround")
            add_custom_command(OUTPUT ${EOKAS_APK_FILE} APPEND
                    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libs/lib/${ANDROID_ABI}/wrap.sh libs/lib/${ANDROID_ABI}/wrap.sh
            )
        else()
            message(WARNING "Without custom wrap script, some Android versions will break due to Android bug")
        endif()

        if(ENABLE_ASAN)
            set(ASAN_ABI_TAG "")
            if(ANDROID_ABI STREQUAL "armeabi-v7a")
                set(ASAN_ABI_TAG "arm")
            elseif(ANDROID_ABI STREQUAL "arm64-v8a")
                set(ASAN_ABI_TAG "aarch64")
            else()
                message(WARNING "Unknown ABI ${ANDROID_ABI}, libasan tag unknown")
            endif()

            file(GLOB ASAN_LIBRARIES "${ANDROID_TOOLCHAIN_ROOT}/lib64/clang/*/lib/linux/libclang_rt.asan-${ASAN_ABI_TAG}-android.so")
            list(SORT ASAN_LIBRARIES)

            list(GET ASAN_LIBRARIES -1 ASAN_LIBRARY)

            set(WRAP_SCRIPT "${ANDROID_NDK_ROOT_PATH}/wrap.sh/asan.sh")

            string(REPLACE "\\" "/" WRAP_SCRIPT "${WRAP_SCRIPT}")
            string(REPLACE "\\" "/" ASAN_LIBRARY "${ASAN_LIBRARY}")

            # Copy in the wrap script and libasan library
            if(${WRAP_SCRIPT} STREQUAL "" OR NOT EXISTS ${WRAP_SCRIPT})
                message(WARNING "Wrap script couldn't be found in NDK, you will need to manually create one and re-generate apk")
            elseif(${ASAN_LIBRARY} STREQUAL "" OR NOT EXISTS ${ASAN_LIBRARY})
                message(WARNING "libasan library couldn't be found in NDK, you will need to manually copy it in and re-generate apk")
            else()
                if(ENABLE_CUSTOM_WRAP_SCRIPT)
                    message(STATUS "Chaining to wrap script ${WRAP_SCRIPT} and libasan library ${ASAN_LIBRARY}")
                    add_custom_command(OUTPUT ${EOKAS_APK_FILE} APPEND
                            COMMAND ${CMAKE_COMMAND} -E copy ${WRAP_SCRIPT} libs/lib/${ANDROID_ABI}/asan.sh
                            COMMAND ${CMAKE_COMMAND} -E copy ${ASAN_LIBRARY} libs/lib/${ANDROID_ABI}/
                    )
                else()
                    message(STATUS "Directly using wrap script ${WRAP_SCRIPT} and libasan library ${ASAN_LIBRARY}")
                    add_custom_command(OUTPUT ${EOKAS_APK_FILE} APPEND
                            COMMAND ${CMAKE_COMMAND} -E copy ${WRAP_SCRIPT} libs/lib/${ANDROID_ABI}/wrap.sh
                            COMMAND ${CMAKE_COMMAND} -E copy ${ASAN_LIBRARY} libs/lib/${ANDROID_ABI}/
                    )
                endif()
            endif()
        endif()

        set(D8_SCRIPT "${BUILD_TOOLS}/d8${TOOL_SCRIPT_EXTENSION}")
        if(NOT EXISTS ${D8_SCRIPT})
            set(DEX_COMMAND ${BUILD_TOOLS}/dx${TOOL_SCRIPT_EXTENSION} --dex --output=bin/classes.dex ./obj)
        else()
            set(DEX_COMMAND ${D8_SCRIPT} --output ./bin/ ./obj/${EOKAS_APK_JAVA_SRC_DIR}/${ABI_EXTENSION_NAME}/*.class)
        endif()

        add_custom_command(OUTPUT ${EOKAS_APK_FILE} APPEND
                COMMAND ${BUILD_TOOLS}/aapt package -f -m -S res -J src -M AndroidManifest.xml -I ${ANDROID_JAR}
                COMMAND ${JAVA_BIN}/javac -d ./obj -source 1.7 -target 1.7 -bootclasspath ${RT_JAR} -classpath "${CLASS_PATH}" -sourcepath src src/${EOKAS_APK_JAVA_SRC_DIR}/*.java
                COMMAND ${DEX_COMMAND}
                COMMAND ${BUILD_TOOLS}/aapt package -f -M AndroidManifest.xml --version-code ${APK_VERSION_CODE} --version-name ${APK_VERSION_NAME} -S res -I ${ANDROID_JAR} -F eokas-out-unaligned.apk bin libs
                COMMAND ${BUILD_TOOLS}/zipalign -f 4 eokas-out-unaligned.apk eokas-out.apk
                COMMAND ${BUILD_TOOLS}/apksigner${TOOL_SCRIPT_EXTENSION} sign --ks ${KEYSTORE} --ks-pass pass:android --key-pass pass:android --ks-key-alias rdocandroidkey eokas-out.apk
                COMMAND ${CMAKE_COMMAND} -E copy eokas-out.apk ${EOKAS_APK_FILE})

    endif()
endfunction()