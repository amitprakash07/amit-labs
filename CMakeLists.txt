cmake_minimum_required(VERSION 3.25.0)

set(PROJECT_NAME "amit-labs")
message("Generating project files in ${CMAKE_BINARY_DIR}")

#inlcude all the CMake global variables
include(${CMAKE_SOURCE_DIR}/cmake/build_global_defs.cmake)

message(DEBUG "Project CMake module dir is ${PROJECT_CMAKE_MODULE_DIR}")
include(${PROJECT_CMAKE_MODULE_DIR}/build_options.cmake)
include(${PROJECT_CMAKE_MODULE_DIR}/macros.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_CMAKE_MODULE_DIR})

set(USE_CXX_COMPILER ${DEFAULT_CXX_COMPILER})
set(USE_C_COMPILER ${DEFAULT_C_COMPILER})

if(UNIX)
    if(${USE_GCC})
        set(USE_CXX_COMPILER gcc)
    endif()
elseif(WIN32)
    if(${USE_MSVC})
        set(USE_CXX_COMPILER msvc)
    endif()
endif()

include(${PROJECT_CMAKE_MODULE_DIR}/build_config.cmake)
message("Processing ${PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR})")
project(${PROJECT_NAME} LANGUAGES C CXX)

# Required for CMake version < 3.29
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)
set(JSON_MultipleHeaders OFF)
set(JsonParser_Version "v3.12.0" CACHE STRING "Json Parser version" FORCE)
set(JsonParser_Path "${PROJECT_EXTERNAL_SOURCE_DIR}/json_parser/${JsonParser_Version}/" CACHE PATH "Json Parser path" FORCE)
FetchContent_Declare(
    JsonParser
    URL https://github.com/nlohmann/json/releases/download/${JsonParser_Version}/json.tar.xz
    SOURCE_DIR ${JsonParser_Path})
FetchContent_MakeAvailable(JsonParser)

# Bitmap fonts
FetchContent_Declare(
    font8x8
    GIT_REPOSITORY https://github.com/dhepper/font8x8
    GIT_TAG 8e279d2d864e79128e96188a6b9526cfa3fbfef9
    SOURCE_DIR "${PROJECT_EXTERNAL_SOURCE_DIR}/font8x8/")
FetchContent_MakeAvailable(font8x8)


add_subdirectory(${PROJECT_SRC_DIR}/core)
add_subdirectory(${PROJECT_SRC_DIR}/engine)
add_subdirectory(${PROJECT_SRC_DIR}/tools/hex2bin)
add_subdirectory(${PROJECT_SRC_DIR}/projects/cpu_rasterizer)
add_subdirectory(${PROJECT_SRC_DIR}/projects/obj_loader)
add_subdirectory(${PROJECT_SRC_DIR}/sandbox)
message("End Processing ${PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR})")
