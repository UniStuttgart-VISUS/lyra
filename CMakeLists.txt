# CMakeLists.txt
# Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

cmake_minimum_required(VERSION 3.21.0)

project(lyra)

include(CMakeDependentOption)
include(GNUInstallDirs)

# User-configurable options.
cmake_dependent_option(LYRA_BuildTests "Build unit tests." ON WIN32 OFF)
cmake_dependent_option(LYRA_Profile "Build with profiling information." OFF WIN32 OFF)

# The library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lyra)

# Unit tests
if (LYRA_BuildTests)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()

# A demo application
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/demo)
