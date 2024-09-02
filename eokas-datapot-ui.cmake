
set(EOKAS_TARGET_NAME "datapot-ui")
set(EOKAS_TARGET_DIR "${EOKAS_SOURCE_DIR}/datapot-ui")

message("============================================================================")
message("EOKAS_TARGET_NAME = ${EOKAS_TARGET_NAME}")
message("EOKAS_TARGET_DIR = ${EOKAS_TARGET_DIR}")

set(EOKAS_HEADER_DIRS
        "${EOKAS_PROJECT_DIR}/deps/vulkan/include"
        "${EOKAS_PROJECT_DIR}/deps/glfw/include"
        "${EOKAS_PROJECT_DIR}/deps/FreeType2/include"

        "${EOKAS_SOURCE_DIR}"
        "${EOKAS_TARGET_DIR}"
        "${EOKAS_TARGET_DIR}/imgui"
        "${EOKAS_TARGET_DIR}/implot"
)

set(EOKAS_LIBRARY_DIRS
        "${EOKAS_PROJECT_DIR}/deps/vulkan/lib/${EOKAS_OS_NAME}"
        "${EOKAS_PROJECT_DIR}/deps/glfw/lib/${EOKAS_OS_NAME}"
        "${EOKAS_PROJECT_DIR}/deps/FreeType2/lib/${EOKAS_OS_NAME}"
)


file(GLOB EOKAS_HEADER_FILES
        "${EOKAS_TARGET_DIR}/*.h"
        "${EOKAS_TARGET_DIR}/imgui/*.h"
        "${EOKAS_TARGET_DIR}/implot/*.h"
        "${EOKAS_TARGET_DIR}/imwidgets/*.h"
        "${EOKAS_TARGET_DIR}/app/*.h"
)

file(GLOB EOKAS_SOURCE_FILES
        "${EOKAS_TARGET_DIR}/*.cpp"
        "${EOKAS_TARGET_DIR}/imgui/*.cpp"
        "${EOKAS_TARGET_DIR}/implot/*.cpp"
        "${EOKAS_TARGET_DIR}/imwidgets/*.cpp"
        "${EOKAS_TARGET_DIR}/app/*.cpp"
)

set(EOKAS_LIBRARY_FILES
    "base" "datapot" "vulkan-1" "glfw3" "freetype"
)

message("EOKAS_HEADER_DIRS = ${EOKAS_HEADER_DIRS}")
message("EOKAS_LIBRARY_DIRS = ${EOKAS_LIBRARY_DIRS}")
message("EOKAS_HEADER_FILES = ${EOKAS_HEADER_FILES}")
message("EOKAS_SOURCE_FILES = ${EOKAS_SOURCE_FILES}")
message("EOKAS_LIBRARY_FILES = ${EOKAS_LIBRARY_FILES}")

add_executable(${EOKAS_TARGET_NAME} ${EOKAS_HEADER_FILES} ${EOKAS_SOURCE_FILES})
target_include_directories(${EOKAS_TARGET_NAME} PRIVATE ${EOKAS_HEADER_DIRS})
target_link_directories(${EOKAS_TARGET_NAME} PRIVATE ${EOKAS_LIBRARY_DIRS})
target_link_libraries(${EOKAS_TARGET_NAME} ${EOKAS_LIBRARY_FILES})


install(TARGETS ${EOKAS_TARGET_NAME} DESTINATION bin/${EOKAS_OS_NAME}/${CMAKE_BUILD_TYPE})

