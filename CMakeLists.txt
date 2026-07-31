cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON)

# `import std;` is experimental in CMake — the UUID acknowledges the opt-in.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

if(APPLE)
    get_filename_component(_cxx_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_cxx_root "${_cxx_bin}" DIRECTORY)
    if(EXISTS "${_cxx_root}/lib/c++/libc++.modules.json")
        set(CMAKE_CXX_STDLIB_MODULES_JSON "${_cxx_root}/lib/c++/libc++.modules.json")
    endif()
endif()

project(JupiterEngine LANGUAGES CXX)

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
endif()

# --- Output directories ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_Output/${JUPITER_PRESET}")

# --- Executable ---
add_executable(JupiterEngine Source/Main.cpp)

file(GLOB_RECURSE JUPITER_MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cppm")

target_sources(JupiterEngine
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${JUPITER_MODULE_SOURCES}
)

if(APPLE)
    get_filename_component(_cxx_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_cxx_root "${_cxx_bin}" DIRECTORY)
    if(EXISTS "${_cxx_root}/lib/c++/libc++.dylib")
        set_target_properties(JupiterEngine PROPERTIES
            BUILD_RPATH "${_cxx_root}/lib/c++"
            INSTALL_RPATH "${_cxx_root}/lib/c++")
    endif()
endif()
