cmake_minimum_required(VERSION 3.25.0)

set(CORE_PROJECT_NAME sdl_windowing)
project(${CORE_PROJECT_NAME})
message("Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")

set(CORE_SDL_WINDOWING_SOURCES
    color_buffer_window.h
    color_buffer_window.cc
    output_path.h
    output_path.cc
    settings_panel.h
    settings_panel.cc)

add_library(${CORE_PROJECT_NAME} ${CORE_SDL_WINDOWING_SOURCES})

target_link_libraries(${CORE_PROJECT_NAME}
    PUBLIC
        graphics
        common
        dear_imgui
        ${PROJECT_SDL3_TARGET})

set_target_properties(${CORE_PROJECT_NAME}
    PROPERTIES
    FOLDER "Core"
    VS_SOLUTION_FOLDER "Core")

message("End Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")
