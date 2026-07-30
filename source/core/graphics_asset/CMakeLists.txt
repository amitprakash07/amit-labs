cmake_minimum_required(VERSION 3.25.0)

set(CORE_PROJECT_NAME graphics_asset)
project(${CORE_PROJECT_NAME})
message("Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")

set(CORE_GRAPHICS_ASSET_SOURCES
    obj_adapter.h
    obj_adapter.cc)

add_library(${CORE_PROJECT_NAME} ${CORE_GRAPHICS_ASSET_SOURCES})

target_link_libraries(${CORE_PROJECT_NAME} PUBLIC asset_loading graphics_common)

set_target_properties(${CORE_PROJECT_NAME}
    PROPERTIES
    FOLDER "Core"
    VS_SOLUTION_FOLDER "Core")

message("End Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")
