# CMakeLists.txt
# Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

project(tests)


# Collect all test cases.
file(GLOB_RECURSE TestFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")

# Create the test executable.
add_executable(${PROJECT_NAME} ${TestFiles})
target_include_directories(${PROJECT_NAME} PRIVATE ${LyraTestInclude})
target_link_libraries(${PROJECT_NAME} PRIVATE sysinfo)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)
target_link_libraries(${PROJECT_NAME} PRIVATE gmock_main)

# Grab the library and copy it to the test output directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sysinfo> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Discover the test cases such that the Test Explorer knows about it.
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
