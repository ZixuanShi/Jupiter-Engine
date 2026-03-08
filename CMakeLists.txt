# Copyright Jupiter Technologies, Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.31)
project(Engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Restrict to the same three configurations used by the old Premake setup.
# Must be set before any add_subdirectory() so it propagates to clients.
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Dev;Release" CACHE STRING "Build configurations" FORCE)
else()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Dev" "Release")
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build configuration" FORCE)
    endif()
endif()

# Map the custom "Dev" config to RelWithDebInfo-equivalent compiler flags.
# CMake only auto-initialises flags for its built-in config names, so we
# initialise Dev the same way CMake initialises RelWithDebInfo.
foreach(lang C CXX)
    set(CMAKE_${lang}_FLAGS_DEV "${CMAKE_${lang}_FLAGS_RELWITHDEBINFO}"
        CACHE STRING "Flags for Dev configuration" FORCE)
endforeach()
set(CMAKE_EXE_LINKER_FLAGS_DEV    "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}"    CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_DEV "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}" CACHE STRING "" FORCE)
set(CMAKE_STATIC_LINKER_FLAGS_DEV "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO}" CACHE STRING "" FORCE)

# ─── Source file collection ───────────────────────────────────────────────────

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/**/*.h"
)
file(GLOB_RECURSE ENGINE_CXX_MODS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/**/*.cppm"
)

# ─── Engine static library ────────────────────────────────────────────────────

add_library(Engine STATIC ${ENGINE_SOURCES})

# Redirect the built .lib to <engine_root>/_Output/Engine_<platform>_<config>_Output/
# matching premake's targetdir (g_jupiterDir .. "_Output/%{prj.name}_" .. g_outputPath .. "_Output").
foreach(config Debug Dev Release)
    string(TOUPPER ${config} CONFIG_UPPER)
    set_target_properties(Engine PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER}
            "${CMAKE_CURRENT_SOURCE_DIR}/_Output/Engine_${JPT_PLATFORM}_${config}_Output"
    )
endforeach()

# Redirect intermediate files (.obj) to match premake's objdir pattern:
# g_jupiterDir .. "_Output/Engine_<platform>_<config>_Intermediate"
# ${JPT_PLATFORM} is a CMake configure-time string; $(Configuration) is an MSBuild macro.
if(MSVC)
    set_target_properties(Engine PROPERTIES
        VS_INTERMEDIATE_DIRECTORY
            "${CMAKE_CURRENT_SOURCE_DIR}/_Output/Engine_${JPT_PLATFORM}_$(Configuration)_Intermediate/")
endif()

# Explicitly require C++20 on the target (not just via CMAKE_CXX_STANDARD,
# which only applies to targets created in this directory scope).
target_compile_features(Engine PUBLIC cxx_std_20)

target_sources(Engine PUBLIC
    FILE_SET CXX_MODULES FILES ${ENGINE_CXX_MODS}
)

# Mirror the Source/ directory tree in the VS Solution Explorer,
# matching the folder structure that Premake generated.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    FILES ${ENGINE_SOURCES} ${ENGINE_CXX_MODS})

# ─── Include directories ──────────────────────────────────────────────────────

target_include_directories(Engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Vulkan/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLFW/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/DX12/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/tiny_obj/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glm/Include"
)

# ─── Compile definitions ──────────────────────────────────────────────────────

# Global defines (replaces Premake's global defines block)
target_compile_definitions(Engine PUBLIC
    "JPT_ENGINE_DIR_W=L\"${CMAKE_CURRENT_SOURCE_DIR}/\""
    NOMINMAX
    GLFW_STATIC
)

# Configuration defines (replaces Premake filter "configurations:*")
target_compile_definitions(Engine PUBLIC
    $<$<CONFIG:Debug>:IS_CONFIG_DEBUG>
    $<$<CONFIG:Dev>:IS_CONFIG_DEV>
    $<$<CONFIG:Release>:IS_CONFIG_RELEASE>
)

# Platform defines (replaces Premake filter "platforms:Windows_*")
# JPT_PLATFORM mirrors premake's g_outputPath platform segment (e.g. "Windows_Client").
# Stored as a CACHE variable so JupiterEngineClient.cmake can read it from the parent scope.
if(WIN32)
    set(JPT_PLATFORM "Windows_Client" CACHE STRING "Jupiter platform identifier" FORCE)
    # UNICODE/_UNICODE make Win32 APIs resolve to the W (wide-char) variants,
    # matching the default Visual Studio "Unicode Character Set" project setting
    # that Premake sets automatically.
    target_compile_definitions(Engine PUBLIC
        IS_PLATFORM_WINDOWS
        UNICODE
        _UNICODE
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(JPT_PLATFORM "Linux_Client" CACHE STRING "Jupiter platform identifier" FORCE)
    target_compile_definitions(Engine PUBLIC IS_PLATFORM_LINUX)
elseif(APPLE AND NOT IOS)
    set(JPT_PLATFORM "Mac_Client" CACHE STRING "Jupiter platform identifier" FORCE)
    target_compile_definitions(Engine PUBLIC IS_PLATFORM_MAC)
elseif(ANDROID)
    set(JPT_PLATFORM "Android_Client" CACHE STRING "Jupiter platform identifier" FORCE)
    target_compile_definitions(Engine PUBLIC IS_PLATFORM_ANDROID)
elseif(IOS)
    set(JPT_PLATFORM "iOS_Client" CACHE STRING "Jupiter platform identifier" FORCE)
    target_compile_definitions(Engine PUBLIC IS_PLATFORM_IOS)
endif()

# ─── Compiler flags ───────────────────────────────────────────────────────────

if(MSVC)
    # /W4 = "Extra" warnings, /MP = multi-processor compilation
    target_compile_options(Engine PUBLIC /W4 /MP)

    # Static runtime (/MT or /MTd) — replaces Premake "staticruntime On"
    set_property(TARGET Engine PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Fatal warnings in Release — replaces Premake "fatalwarnings All"
    target_compile_options(Engine PUBLIC
        $<$<CONFIG:Release>:/WX>)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(Engine PUBLIC -Wall -Wextra)
endif()

# ─── Link libraries ───────────────────────────────────────────────────────────

if(WIN32)
    target_link_libraries(Engine PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Vulkan/Libs/vulkan-1.lib"
        d3d12
        dxgi
        d3dcompiler
        "$<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLFW/Libs/glfw3_Debug.lib>"
        "$<$<NOT:$<CONFIG:Debug>>:${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLFW/Libs/glfw3_Release.lib>"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Vulkan REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(Engine PUBLIC Vulkan::Vulkan glfw)
elseif(APPLE)
    find_package(Vulkan REQUIRED)   # Provided by MoltenVK
    find_package(glfw3 REQUIRED)
    target_link_libraries(Engine PUBLIC
        Vulkan::Vulkan
        glfw
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
    )
endif()
