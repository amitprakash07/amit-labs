cmake_minimum_required(VERSION 3.25.0)

set(CORE_PROJECT_NAME graphics)
project(${CORE_PROJECT_NAME})
message("Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")

#camera.h
#camera.cc
#frame_time.h
#frame_time.cc    
#mesh_object.h
#mesh_object.cc
#object_interface.h
#scene.h
#scene.cc

set(CORE_GRAPHICS_SOURCES
pixel.h
viewport.h
color.cc
text_overlay.h
text_overlay.cc
render_stats.h
render_context.h
image_2d.h
image_2d.cc
render_primitives.cc
image_generator.h
image_generator.cc
texture.h
barycentric_interpolation.h
transform.h
projection.h)

file(GLOB FONT8X8_HEADERS CONFIGURE_DEPENDS
  "${PROJECT_EXTERNAL_SOURCE_DIR}/font8x8/*.h")

add_library(${CORE_PROJECT_NAME} ${CORE_GRAPHICS_SOURCES})

target_link_libraries(${CORE_PROJECT_NAME} PUBLIC graphics_common)
target_sources(${CORE_PROJECT_NAME} PRIVATE ${FONT8X8_HEADERS})
set_source_files_properties(${FONT8X8_HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("external/font8x8" FILES ${FONT8X8_HEADERS})

set_target_properties(${CORE_PROJECT_NAME}
    PROPERTIES
    FOLDER "Core"
    VS_SOLUTION_FOLDER "Core")

target_include_directories(${CORE_PROJECT_NAME} PRIVATE ${PROJECT_EXTERNAL_SOURCE_DIR}/font8x8/)

message("End Processing ${CORE_PROJECT_NAME} - ${CMAKE_CURRENT_LIST_DIR}")


