# A: Project Setup
cmake_minimum_required(VERSION 3.31)
project(nanojsonc VERSION 1.3.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# B: Source library
# 1. Library Definition, Target properties
add_library(nanojsonc STATIC src/object.c src/array.c)
add_library(nanojsonc::nanojsonc ALIAS nanojsonc)
target_compile_features(nanojsonc PRIVATE c_std_11)

# 2. Public Header Paths
target_include_directories(nanojsonc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Include modules for install and config
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# C: Configuration
# 1. Generate Template
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake.in"
"@PACKAGE_INIT@\n\n\
include(\"\${CMAKE_CURRENT_LIST_DIR}/nanojsonc-targets.cmake\")\n\n\
check_required_components(nanojsonc)\n")

# 2. Configure
configure_package_config_file(
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc
        NO_SET_AND_CHECK_MACRO)

# 3. Write version file
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

# D: Installation steps
# 1. Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/nanojsonc
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 2. Install library and export targets
install(TARGETS nanojsonc
        EXPORT nanojsonc-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT nanojsonc_RunTime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT nanojsonc_RunTime
            NAMELINK_COMPONENT nanojsonc_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT nanojsonc_Development)

# Install Export Set
install(EXPORT nanojsonc-targets
        FILE nanojsonc-targets.cmake
        NAMESPACE nanojsonc::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc)

# 4. Install config and version files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake # Install Config
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config-version.cmake # Install version
        DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc)

# Enable AddressSanitizer and UndefinedBehaviorSanitizer (Compiler and Linker)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if (ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")

    target_compile_options(nanojsonc PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all)
    target_link_options(nanojsonc PRIVATE -fsanitize=address,undefined)
endif ()

# Debug-specific compiler flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    target_compile_options(nanojsonc PRIVATE -Werror -Wall)
endif ()

# Tests
option(BUILD_TESTS "Build Tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
