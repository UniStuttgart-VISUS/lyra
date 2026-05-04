# CMakeLists.txt
# Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

project(write)


# Collect all test cases.
file(GLOB_RECURSE TestFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.c")

# Create the test executable.
add_executable(${PROJECT_NAME} ${TestFiles})
target_include_directories(${PROJECT_NAME} PRIVATE ${LyraTestInclude})
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
target_link_libraries(${PROJECT_NAME} PRIVATE autodoc)

# Grab the library and copy it to the test output directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:autodoc> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
