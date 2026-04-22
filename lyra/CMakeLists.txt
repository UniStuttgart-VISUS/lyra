# CMakeLists.txt
# Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

project(liblyra)


# Search for LibICU for handling different encodings.
set(ICU_DEBUG ON)
find_package(ICU COMPONENTS uc data)

# Collect source files.
set(IncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE PublicHeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${IncludeDir}/*.h" "${IncludeDir}/*.inl")

set(SourceDir "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE PrivateHeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SourceDir}/*.h" "${SourceDir}/*.inl")
file(GLOB_RECURSE SourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SourceDir}/*.cpp")

source_group("Header Files\\include" FILES  ${PublicHeaderFiles})
source_group("Header Files\\src" FILES  ${PrivateHeaderFiles})
source_group("Source Files" FILES  ${SourceFiles})

# Define the output.
add_library(${PROJECT_NAME} SHARED ${PublicHeaderFiles} ${PrivateHeaderFiles} ${SourceFiles})

# Configure the compiler
target_include_directories(${PROJECT_NAME} PUBLIC ${IncludeDir})
target_include_directories(${PROJECT_NAME} PRIVATE ${SourceDir})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

# Configure the linker.
if (WIN32)
    target_link_libraries(${PROJECT_NAME} INTERFACE icu)
else ()
    target_link_libraries(${PROJECT_NAME} INTERFACE ICU::uc ICU::data)
endif ()

# Install
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
