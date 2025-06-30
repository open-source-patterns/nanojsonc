cmake_minimum_required(VERSION 3.26)
project(nanojsonc C)

set(CMAKE_C_STANDARD 11)
set(NANOJSONC_VERSION 1.3.0)

add_library(nanojsonc src/object.c src/array.c)
add_library(nanojsonc::nanojsonc ALIAS nanojsonc)
target_compile_features(nanojsonc PRIVATE c_std_11)

target_include_directories(nanojsonc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Config
configure_package_config_file(build/config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake # cmake-build-debug/
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} NO_SET_AND_CHECK_MACRO) # share/

# Version
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        VERSION ${NANOJSONC_VERSION}
        COMPATIBILITY SameMajorVersion)

# Install Config & Version
install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake # cmake-build-debug/
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake # cmake_build-debug/
        DESTINATION
            ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}) # share/

# Create export set to enable import in other CMake Projects
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets # create export set
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)
# Install Export Set
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}-targets.cmake
        NAMESPACE nanojsonc::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}) # lib/cmake/nanojsonc

# Install Headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}) # include/

# Enable AddressSanitizer and UndefinedBehaviorSanitizer (Compiler and Linker)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if (ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")

    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
endif ()

# Debug-specific compiler flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -std=c11)
endif ()

# Tests
option(BUILD_TESTS "Build Tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
