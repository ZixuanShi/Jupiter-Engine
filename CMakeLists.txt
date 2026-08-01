cmake_minimum_required(VERSION 3.30)

# --- C++ standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

# --- Apple: everything that must happen BEFORE project() ---
# Homebrew LLVM lives at <root>/bin/clang++ with its libc++ at <root>/lib/c++.
if(APPLE)
    get_filename_component(_cxx_bin  "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_cxx_root "${_cxx_bin}" DIRECTORY)

    if(EXISTS "${_cxx_root}/lib/c++/libc++.modules.json")
        set(CMAKE_CXX_STDLIB_MODULES_JSON "${_cxx_root}/lib/c++/libc++.modules.json")
    endif()

    # ObjC++ platform layer. Every .mm is globbed on every Apple target and guards its own
    # body with #if IS_PLATFORM_MACOS / IS_PLATFORM_IOS, so there is no per-platform source
    # list. Globbed here purely to keep the Apple logic together.
    file(GLOB_RECURSE JUPITER_OBJCXX_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Source/*.mm")
endif()

# --- Project ---
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

add_executable(JupiterEngine ${JUPITER_SOURCES} ${JUPITER_OBJCXX_SOURCES})

# Lets any unit include platform headers by path from Source/, e.g. #include "Platform/Apple/AppleWindow.h"
target_include_directories(JupiterEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_sources(JupiterEngine
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${JUPITER_MODULE_SOURCES}
)

# --- Platform-specifics ---
if(APPLE)
    target_compile_options(JupiterEngine PRIVATE $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)

    # Foundation is not optional here: Homebrew clang does not auto-link libobjc the way
    # Apple clang does, so without it the link fails on _objc_release / _objc_autoreleasePool*.
    if(JUPITER_PLATFORM STREQUAL "IOS")
        target_link_libraries(JupiterEngine PRIVATE
            "-framework UIKit" "-framework Foundation"
            "-framework Metal" "-framework QuartzCore")
    else()
        target_link_libraries(JupiterEngine PRIVATE
            "-framework Cocoa" "-framework Foundation"
            "-framework Metal" "-framework QuartzCore")
    endif()

    # --- App bundle ---
    # iOS refuses to install anything that is not a bundle. On macOS a bundle is what gives
    # the process a CFBundleIdentifier, without which the system logs a stream of
    # "missing main bundle identifier" / intents-registration failures on every launch.
    set(MACOSX_BUNDLE_EXECUTABLE_NAME      "JupiterEngine")
    set(MACOSX_BUNDLE_BUNDLE_NAME          "Jupiter Engine")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER       "com.jupitertechnologies.engine")
    set(MACOSX_BUNDLE_BUNDLE_VERSION       "1")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")

    if(JUPITER_PLATFORM STREQUAL "IOS")
        if(CMAKE_OSX_SYSROOT MATCHES "[Ss]imulator")
            set(JUPITER_IOS_PLATFORM_NAME "iPhoneSimulator")
        else()
            set(JUPITER_IOS_PLATFORM_NAME "iPhoneOS")
        endif()
        set(JUPITER_INFO_PLIST "${CMAKE_SOURCE_DIR}/Build/Apple/IOSInfo.plist.in")
    else()
        set(JUPITER_INFO_PLIST "${CMAKE_SOURCE_DIR}/Build/Apple/MacOSInfo.plist.in")
    endif()

    set_target_properties(JupiterEngine PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${JUPITER_INFO_PLIST}")

    # macOS links Homebrew's libc++, so it needs the rpath. iOS resolves against the
    # system /usr/lib/libc++.1.dylib instead, where a Homebrew path would be dead weight
    # baked into a bundle that ships to a device.
    if(NOT JUPITER_PLATFORM STREQUAL "IOS" AND EXISTS "${_cxx_root}/lib/c++/libc++.dylib")
        set_target_properties(JupiterEngine PROPERTIES
            BUILD_RPATH "${_cxx_root}/lib/c++"
            INSTALL_RPATH "${_cxx_root}/lib/c++")
    endif()
endif()
