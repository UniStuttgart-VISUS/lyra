# CMakeLists.txt
# Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

project(sysinfo)


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
source_group("Source Files\\src" FILES  ${SourceFiles})

# Define the output.
add_library(${PROJECT_NAME} SHARED ${PublicHeaderFiles} ${PrivateHeaderFiles} ${SourceFiles})

# Configure the compiler
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${IncludeDir}>
    PRIVATE
        $<BUILD_INTERFACE:${SourceDir}>)
target_compile_definitions(${PROJECT_NAME} PRIVATE LYRA_EXPORTS)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
endif ()

# Configure the linker.
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE dxgi windowscodecs)
    target_link_libraries(${PROJECT_NAME} PRIVATE icu)
    target_link_libraries(${PROJECT_NAME} PRIVATE SetupAPI)
    target_link_libraries(${PROJECT_NAME} PRIVATE WIL::WIL)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE ICU::uc ICU::data)
endif ()

# Publish the private headers for testing.
if (LYRA_BuildTests)
    set(LyraTestInclude ${SourceDir} PARENT_SCOPE)
endif ()

# Install
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    #PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Config.cmake
    #NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
