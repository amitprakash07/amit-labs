cmake_minimum_required(VERSION 3.25.0)

set(PROJECT_NAME coding_problems)
project(${PROJECT_NAME})

set(CODING_PROBLEMS_ROOT_SOURCES
    interface.h
    list.h
    main.cc)

set(CODING_PROBLEMS_LEETCODE_SOURCES
    leetcode/k-radius_averages.h
    leetcode/list.h
    leetcode/maximum_sub_array.h
    leetcode/min_start_value.h
    leetcode/remove_duplicate_sorted_array_2.h
    leetcode/sorted_square_array.h
    leetcode/triplet_sum.h
    leetcode/unique_email_address.h)

set(CODING_PROBLEMS_OTHERS_SOURCES
    others/count_triangles.h
    others/list.h
    others/max_substring_length.h
    others/passing_year_books.h
    others/revenue_milestones.h
    others/reverse_to_make_equal.h
    others/rotational_cipher.h
    others/zip_link_list.h)

set(CODING_PROBLEMS_SOURCES
    ${CODING_PROBLEMS_ROOT_SOURCES}
    ${CODING_PROBLEMS_LEETCODE_SOURCES}
    ${CODING_PROBLEMS_OTHERS_SOURCES})

include_directories(${PROJECT_SRC_DIR})
add_executable(${PROJECT_NAME} ${CODING_PROBLEMS_SOURCES})

source_group("" FILES ${CODING_PROBLEMS_ROOT_SOURCES})
source_group("LeetCode" FILES ${CODING_PROBLEMS_LEETCODE_SOURCES})
source_group("Others" FILES ${CODING_PROBLEMS_OTHERS_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE core)

add_dependencies(${PROJECT_NAME} core)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Sandbox"
                                                 VS_SOLUTION_FOLDER "Sandbox")
