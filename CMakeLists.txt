# CMakeLists.txt
# Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

cmake_minimum_required(VERSION 3.21.0)

project(lyra)

include(CMakeDependentOption)
include(FetchContent)
include(GNUInstallDirs)

# User-configurable options.
option(LYRA_BuildTests "Build unit tests." OFF)
cmake_dependent_option(LYRA_Profile "Build with profiling information." OFF WIN32 OFF)

# Dependencies
if (WIN32)
    FetchContent_Declare(wil
        URL "https://github.com/microsoft/wil/archive/refs/tags/v1.0.250325.1.zip"
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    option(FAST_BUILD "" OFF)
    option(WIL_BUILD_PACKAGING "" OFF)
    option(WIL_BUILD_TESTS "" OFF)
    FetchContent_MakeAvailable(wil)
endif ()

if (LYRA_BuildTests)
    FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()

# JSON for Modern C++
FetchContent_Declare(nlohmann_json
    URL "https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.zip"
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
option(JSON_Install "" ON)
FetchContent_MakeAvailable(nlohmann_json)

# Signal that we need to access private APIs for testing.
if (LYRA_BuildTests)
    add_compile_definitions(LYRA_EXPORT_TESTS)
endif ()

# The library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lyra)

# Unit tests
if (LYRA_BuildTests)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()

# A demo application
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/demo)
