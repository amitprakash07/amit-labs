cmake_minimum_required(VERSION 3.25.0)

set(CORE_PROJECT_NAME core)
message("Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")
project(${CORE_PROJECT_NAME})

find_package(AgilitySDK REQUIRED)
include_directories(${PROJECT_SRC_DIR})
include_directories(${AgilitySdk_Include})

add_subdirectory(common)
add_subdirectory(maths)
add_subdirectory(graphics_common)
add_subdirectory(graphics)
add_subdirectory(asset_loading)
add_subdirectory(cpu_renderer)
add_subdirectory(graphics_asset)
# Opt-in SDL window/input helper — apps that need a present loop link `sdl_windowing` explicitly
# so targets that only need core rendering do not pull SDL.
add_subdirectory(sdl_windowing)

add_library(${CORE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CORE_PROJECT_NAME} INTERFACE
    asset_loading
    common
    cpu_renderer
    graphics
    graphics_asset
    graphics_common
    maths)

set_target_properties(${CORE_PROJECT_NAME}
    PROPERTIES
    FOLDER "Core"
    VS_SOLUTION_FOLDER "Core")
message("End Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")