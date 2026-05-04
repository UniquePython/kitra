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
# ============================================================

add_library(cinder_math INTERFACE)
add_library(Cinder::Math ALIAS cinder_math)

target_include_directories(cinder_math
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(cinder_math INTERFACE m)

# ============================================================
# cinder — main static library
# ============================================================

add_library(cinder STATIC
    src/cinder_subsystem.c
    src/cinder_color.c
    src/cinder_window.c
    src/cinder_loop.c
    src/cinder_timer.c
    src/cinder_input.c
    src/cinder_draw.c
    src/cinder_texture.c
    src/cinder_text.c
    src/cinder_error.c
    src/cinder_rng.c
)
add_library(Cinder::Cinder ALIAS cinder)

target_include_directories(cinder
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(cinder PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wdouble-promotion
    -Wformat=2
    -Wundef
)

target_link_options(cinder PRIVATE -Wl,--allow-shlib-undefined)

target_link_libraries(cinder
    PUBLIC  Cinder::Math
    PRIVATE
        PkgConfig::SDL2
        PkgConfig::SDL2_IMAGE
        PkgConfig::SDL2_GFX
        PkgConfig::SDL2_TTF
)

# ============================================================
# Examples
# ============================================================

foreach(EXAMPLE pong snake)
    add_executable(${EXAMPLE} examples/${EXAMPLE}/${EXAMPLE}.c)
    target_link_options(${EXAMPLE} PRIVATE -Wl,--allow-shlib-undefined)
    target_link_libraries(${EXAMPLE} PRIVATE Cinder::Cinder)
    set_target_properties(${EXAMPLE} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endforeach()

# ============================================================
# Install
# ============================================================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS cinder cinder_math
    EXPORT  CinderTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES
    include/cinder.h
    include/cinder_types.h
    include/cinder_math.h
    include/cinder_status.h
    include/cinder_subsystem.h
    include/cinder_window.h
    include/cinder_loop.h
    include/cinder_timer.h
    include/cinder_input.h
    include/cinder_color.h
    include/cinder_draw.h
    include/cinder_texture.h
    include/cinder_text.h
    include/cinder_error.h
    include/cinder_rng.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cinder
)

install(EXPORT CinderTargets
    FILE      CinderTargets.cmake
    NAMESPACE Cinder::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Cinder
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CinderConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

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

# ============================================================
# pkg-config
# ============================================================

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cinder.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cinder.pc"
    @ONLY
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cinder_math.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cinder_math.pc"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cinder.pc"
    "${CMAKE_CURRENT_BINARY_DIR}/cinder_math.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)