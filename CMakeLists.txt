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

# SDL3 window/input layer
set(SDL3_VERSION "release-3.4.12" CACHE STRING "SDL3 version" FORCE)
set(SDL3_PATH "${PROJECT_EXTERNAL_SOURCE_DIR}/SDL/${SDL3_VERSION}/" CACHE PATH "SDL3 path" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Build SDL3 shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL3 static library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build SDL3 tests" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL3 test library" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Build SDL3 examples" FORCE)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG ${SDL3_VERSION}
    SOURCE_DIR ${SDL3_PATH})
FetchContent_MakeAvailable(SDL3)

foreach(SDL3_GENERATED_TARGET SDL3-static SDL3-shared)
    if(TARGET ${SDL3_GENERATED_TARGET})
        set_target_properties(${SDL3_GENERATED_TARGET}
            PROPERTIES
            FOLDER "External"
            VS_SOLUTION_FOLDER "External")
    endif()
endforeach()

if(TARGET SDL3::SDL3)
    set(PROJECT_SDL3_TARGET SDL3::SDL3)
elseif(TARGET SDL3::SDL3-static)
    set(PROJECT_SDL3_TARGET SDL3::SDL3-static)
else()
    message(FATAL_ERROR "SDL3 target was not created")
endif()

# Dear ImGui debug UI with SDL3 platform and DX12 renderer backends.
set(DearImGui_VERSION "v1.92.9" CACHE STRING "Dear ImGui version" FORCE)
set(DearImGui_PATH "${PROJECT_EXTERNAL_SOURCE_DIR}/dear_imgui/${DearImGui_VERSION}/" CACHE PATH "Dear ImGui path" FORCE)
FetchContent_Declare(
    DearImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG ${DearImGui_VERSION}
    SOURCE_DIR ${DearImGui_PATH})
FetchContent_MakeAvailable(DearImGui)

set(DEAR_IMGUI_SOURCES
    ${dearimgui_SOURCE_DIR}/imgui.cpp
    ${dearimgui_SOURCE_DIR}/imgui_demo.cpp
    ${dearimgui_SOURCE_DIR}/imgui_draw.cpp
    ${dearimgui_SOURCE_DIR}/imgui_tables.cpp
    ${dearimgui_SOURCE_DIR}/imgui_widgets.cpp
    ${dearimgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp)

if(WIN32)
    list(APPEND DEAR_IMGUI_SOURCES ${dearimgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp)
endif()

add_library(dear_imgui STATIC ${DEAR_IMGUI_SOURCES})
target_include_directories(dear_imgui
    PUBLIC
        ${dearimgui_SOURCE_DIR}
        ${dearimgui_SOURCE_DIR}/backends)
target_link_libraries(dear_imgui PUBLIC ${PROJECT_SDL3_TARGET})

if(WIN32)
    target_link_libraries(dear_imgui PUBLIC d3d12.dll dxgi.dll)
endif()

set_target_properties(dear_imgui
    PROPERTIES
    FOLDER "External"
    VS_SOLUTION_FOLDER "External")


add_compile_definitions(ASSET_DIR="${PROJECT_DATA_DIR}")

add_subdirectory(${PROJECT_SRC_DIR}/core)
add_subdirectory(${PROJECT_SRC_DIR}/engine)
add_subdirectory(${PROJECT_SRC_DIR}/tools/hex2bin)
add_subdirectory(${PROJECT_SRC_DIR}/projects/cpu_rasterizer)
add_subdirectory(${PROJECT_SRC_DIR}/projects/obj_loader)
add_subdirectory(${PROJECT_SRC_DIR}/sandbox)
message("End Processing ${PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR})")
