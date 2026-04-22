# CMakeLists.txt
# Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

project(tests)

# Collect all test sources to be run by a single exectuable.
file(GLOB_RECURSE TestFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")

create_test_sourcelist(Tests ${PROJECT_NAME}.cpp ${TestFiles})

# Create the test executable running all the tests in the source list.
add_executable(${PROJECT_NAME} ${Tests})
target_link_libraries(${PROJECT_NAME} PRIVATE sysinfo)

# Grab the library and copy it to the test output directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:sysinfo> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

foreach (t ${TestFiles})
    get_filename_component(TestName ${t} NAME_WE)
    add_test(NAME ${TestName} COMMAND ${PROJECT_NAME} ${TestName})
endforeach ()
