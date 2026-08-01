cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

# Homebrew LLVM lives at <root>/bin/clang++ with its libc++ at <root>/lib/c++.
# Derived once here, before project(), because CMAKE_CXX_STDLIB_MODULES_JSON has to be set
# before the compiler is configured. Plain variables survive project(), so the target rpath
# below reuses _cxx_root rather than recomputing it.
if(APPLE)
    get_filename_component(_cxx_bin  "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_cxx_root "${_cxx_bin}" DIRECTORY)

    if(EXISTS "${_cxx_root}/lib/c++/libc++.modules.json")
        set(CMAKE_CXX_STDLIB_MODULES_JSON "${_cxx_root}/lib/c++/libc++.modules.json")
    endif()
endif()

project(JupiterEngine LANGUAGES CXX OBJCXX)

option(JUPITER_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

# --- Platform defines ---
set(JUPITER_PLATFORMS WINDOWS MACOS IOS ANDROID LINUX)

if(WIN32)
    set(JUPITER_PLATFORM WINDOWS)
elseif(APPLE)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(JUPITER_PLATFORM IOS)
    else()
        set(JUPITER_PLATFORM MACOS)
    endif()
elseif(ANDROID)
    set(JUPITER_PLATFORM ANDROID)
elseif(UNIX)
    set(JUPITER_PLATFORM LINUX)
endif()

foreach(_platform ${JUPITER_PLATFORMS})
    if(_platform STREQUAL JUPITER_PLATFORM)
        add_compile_definitions(IS_PLATFORM_${_platform}=1)
    else()
        add_compile_definitions(IS_PLATFORM_${_platform}=0)
    endif()
endforeach()

# --- Config defines ---
set(JUPITER_CONFIGS DEBUG DEV RELEASE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(JUPITER_CONFIG DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(JUPITER_CONFIG DEV)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(JUPITER_CONFIG RELEASE)
endif()

foreach(_config ${JUPITER_CONFIGS})
    if(_config STREQUAL JUPITER_CONFIG)
        add_compile_definitions(IS_CONFIG_${_config}=1)
    else()
        add_compile_definitions(IS_CONFIG_${_config}=0)
    endif()
endforeach()

# --- Compiler flags ---
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fcolor-diagnostics -fansi-escape-codes)
    add_compile_options(-Wall -Wextra -Wpedantic)

    if(JUPITER_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()
endif()

# --- Output directories ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_Output/${JUPITER_PRESET}")

# --- Executable ---
file(GLOB_RECURSE JUPITER_SOURCES        CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cpp")
file(GLOB_RECURSE JUPITER_MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cppm")

# ObjC++ platform layer. Every .mm is globbed on every Apple target and guards its own body
# with #if IS_PLATFORM_MACOS / IS_PLATFORM_IOS, so there is no per-platform source list here.
if(APPLE)
    file(GLOB_RECURSE JUPITER_OBJCXX_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.mm")
endif()

add_executable(JupiterEngine ${JUPITER_SOURCES} ${JUPITER_OBJCXX_SOURCES})

# Lets any unit include platform headers by path from Source/, e.g. #include "Platform/Apple/AppleWindow.h"
target_include_directories(JupiterEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_sources(JupiterEngine
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${JUPITER_MODULE_SOURCES}
)

if(APPLE)
    target_compile_options(JupiterEngine PRIVATE $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)

    if(JUPITER_PLATFORM STREQUAL "IOS")
        target_link_libraries(JupiterEngine PRIVATE
            "-framework UIKit" "-framework Foundation"
            "-framework Metal" "-framework QuartzCore")
    else()
        target_link_libraries(JupiterEngine PRIVATE
            "-framework Cocoa"
            "-framework Metal" "-framework QuartzCore")
    endif()

    if(EXISTS "${_cxx_root}/lib/c++/libc++.dylib")
        set_target_properties(JupiterEngine PROPERTIES
            BUILD_RPATH "${_cxx_root}/lib/c++"
            INSTALL_RPATH "${_cxx_root}/lib/c++")
    endif()
endif()
