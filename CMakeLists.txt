cmake_minimum_required(VERSION 3.21)

project(Cinder
    VERSION 0.1.0
    DESCRIPTION "A minimal, beginner-friendly 2D graphics library built on SDL2"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ============================================================
# Dependencies
# ============================================================

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2       REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_GFX   REQUIRED IMPORTED_TARGET SDL2_gfx)
pkg_check_modules(SDL2_TTF   REQUIRED IMPORTED_TARGET SDL2_ttf)

# ============================================================
# cinder_math — header-only interface library
# Math and compiler utilities only. No SDL dependency.
# Users can link against this alone without pulling in SDL.
# ============================================================

add_library(cinder_math INTERFACE)
add_library(Cinder::Math ALIAS cinder_math)

target_include_directories(cinder_math
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# cinder_math uses sqrtf etc, so needs libm on Linux
target_link_libraries(cinder_math INTERFACE m)

# ============================================================
# cinder — main static library
# ============================================================

add_library(cinder STATIC src/cinder.c)
add_library(Cinder::Cinder ALIAS cinder)

target_include_directories(cinder
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(cinder PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wdouble-promotion
    -Wformat=2
    -Wundef
)

# SDL deps are PRIVATE — they do not propagate to the user.
# cinder_math is PUBLIC — users get it transitively.
target_link_libraries(cinder
    PUBLIC  Cinder::Math
    PRIVATE
        PkgConfig::SDL2
        PkgConfig::SDL2_IMAGE
        PkgConfig::SDL2_GFX
        PkgConfig::SDL2_TTF
)

# ============================================================
# Test executables — not installed
# ============================================================

# Example 1 - pong

add_executable(pong src/pong.c)

target_link_options(cinder PRIVATE -Wl,--allow-shlib-undefined)
target_link_options(pong PRIVATE -Wl,--allow-shlib-undefined)

target_link_libraries(pong PRIVATE Cinder::Cinder)

# Dump the binary next to the source for convenience
set_target_properties(pong PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Example 2 - snake

add_executable(snake src/snake.c)

target_link_options(cinder PRIVATE -Wl,--allow-shlib-undefined)
target_link_options(snake PRIVATE -Wl,--allow-shlib-undefined)

target_link_libraries(snake PRIVATE Cinder::Cinder)

# Dump the binary next to the source for convenience
set_target_properties(snake PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# ============================================================
# Install
# ============================================================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install the libraries
install(TARGETS cinder cinder_math
    EXPORT  CinderTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install the headers
install(FILES
    include/cinder.h
    include/cinder_math.h
    include/cinder_compiler.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cinder
)

# Install the export set (lets downstream find_package(Cinder) work)
install(EXPORT CinderTargets
    FILE      CinderTargets.cmake
    NAMESPACE Cinder::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Cinder
)

# Generate and install version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CinderConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Generate and install config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CinderConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CinderConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Cinder
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CinderConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CinderConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Cinder
)