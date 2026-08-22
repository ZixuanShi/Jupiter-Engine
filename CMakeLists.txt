cmake_minimum_required(VERSION 3.30)

# --- C++ standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

# --- Apple toolchain ---
# Must precede project(): CMAKE_CXX_STDLIB_MODULES_JSON is only read during compiler detection.
if(APPLE)
    get_filename_component(_cxx_bin  "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_cxx_root "${_cxx_bin}" DIRECTORY)

    if(EXISTS "${_cxx_root}/lib/c++/libc++.modules.json")
        set(CMAKE_CXX_STDLIB_MODULES_JSON "${_cxx_root}/lib/c++/libc++.modules.json")
    endif()
endif()

# --- Android toolchain ---
# Same timing constraint as above. The NDK ships no std module, and CMake's fallback probe runs
# clang with no --target, so NDK clang answers for its host triple -- measured resolving
# llvm-mingw's std.cppm from PATH, clang 21 paired with clang 20 sources. Vendor/LibCxxModules is
# generated from the NDK's own LLVM commit; see its README, which also documents why the android
# presets carry --target in CMAKE_CXX_FLAGS.
if(CMAKE_TOOLCHAIN_FILE MATCHES "android\\.toolchain\\.cmake")
    set(CMAKE_CXX_STDLIB_MODULES_JSON "${CMAKE_SOURCE_DIR}/Vendor/LibCxxModules/libc++.modules.json")
endif()

# --- Project ---
# C is SDL3's language on every platform. OBJC/OBJCXX only where Apple sources are compiled --
# SDL enables OBJC itself, but doing it here is what makes the preset's pinned compiler apply.
project(JupiterEngine LANGUAGES C CXX)
if(APPLE)
    enable_language(OBJC)
    enable_language(OBJCXX)
endif()

option(JUPITER_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

# Asset staging and iOS signing run through it. `python3` is not a name Windows has.
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# --- Compile defines ---
# Every name gets a define, 0 or 1, so `#if IS_PLATFORM_MACOS` is always a valid expression --
# an `#ifdef` spelling would silently take the false branch on a typo.
function(jpt_define_flags prefix active)
    foreach(_name ${ARGN})
        if(_name STREQUAL active)
            add_compile_definitions(${prefix}${_name}=1)
        else()
            add_compile_definitions(${prefix}${_name}=0)
        endif()
    endforeach()
endfunction()

set(JUPITER_PLATFORMS WINDOWS MACOS IOS ANDROID LINUX)
set(JUPITER_CONFIGS   DEBUG DEV RELEASE)

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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(JUPITER_CONFIG DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(JUPITER_CONFIG DEV)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(JUPITER_CONFIG RELEASE)
endif()

# Quoted, so an undetected platform stays an empty argument rather than shifting the list along
# and crowning WINDOWS the active one.
jpt_define_flags(IS_PLATFORM_ "${JUPITER_PLATFORM}" ${JUPITER_PLATFORMS})
jpt_define_flags(IS_CONFIG_   "${JUPITER_CONFIG}"   ${JUPITER_CONFIGS})

# Form factor, not capability: a phone is touch-first and a desktop is pointer-first, which is what
# UI sizing keys off. Whether a keyboard or mouse is attached is a runtime question -- an iPad takes
# both -- so nothing about input belongs behind these.
set(JUPITER_PC_PLATFORMS     WINDOWS MACOS LINUX)
set(JUPITER_MOBILE_PLATFORMS IOS ANDROID)

if(JUPITER_PLATFORM IN_LIST JUPITER_PC_PLATFORMS)
    add_compile_definitions(IS_PC=1 IS_MOBILE=0)
elseif(JUPITER_PLATFORM IN_LIST JUPITER_MOBILE_PLATFORMS)
    add_compile_definitions(IS_PC=0 IS_MOBILE=1)
else()
    message(FATAL_ERROR "Platform '${JUPITER_PLATFORM}' is in neither JUPITER_PC_PLATFORMS nor JUPITER_MOBILE_PLATFORMS")
endif()

# --- Editor ---
# Whether the editor and the Dear ImGui it draws through are compiled in at all. Not the same
# question as "is this a dev build": a dev build on a platform with no ImGui backend has no editor
# either, and three files used to spell that predicate three different ways.
if(NOT JUPITER_CONFIG STREQUAL "RELEASE" AND APPLE)
    set(JUPITER_EDITOR 1)
else()
    set(JUPITER_EDITOR 0)
endif()

add_compile_definitions(IS_EDITOR=${JUPITER_EDITOR})

# --- Compiler flags ---
# Scoped to the languages this project writes. These are directory-scoped and inherited by every
# add_subdirectory below, and vendored C does not survive them. One generator expression per flag:
# a genex expanding to two flags arrives as a single quoted argument.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fcolor-diagnostics -fansi-escape-codes)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX,OBJCXX>:-Wall>
                        $<$<COMPILE_LANGUAGE:CXX,OBJCXX>:-Wextra>
                        $<$<COMPILE_LANGUAGE:CXX,OBJCXX>:-Wpedantic>)

    if(JUPITER_WARNINGS_AS_ERRORS)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX,OBJCXX>:-Werror>)
    endif()
endif()

# --- Android ---
if(ANDROID)
    # The engine archive's members land in the project's libmain.so.
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    # SDL's jar target globs java sources from android-project/, which Vendor/SDL3 deliberately
    # drops; the vendored copy lives in Vendor/SDL3/Android and is compiled by the gradle project.
    set(SDL_ANDROID_JAR OFF CACHE BOOL "" FORCE)

    # Bionic declares the fortified stdio wrappers and the ctype helpers static inline, which the
    # std module cannot re-export. Both fixes scope to the one TU that compiles std.cppm; the
    # engine keeps _FORTIFY_SOURCE=2. Deferred because CMake synthesizes the target itself --
    # see Vendor/LibCxxModules/README.md.
    function(jpt_fix_android_std_module)
        if(TARGET __cmake_cxx23)
            target_compile_options(__cmake_cxx23 PRIVATE -U_FORTIFY_SOURCE)
            target_compile_definitions(__cmake_cxx23 PRIVATE "__BIONIC_CTYPE_INLINE=inline")
        endif()
    endfunction()
    cmake_language(DEFER CALL jpt_fix_android_std_module)
endif()

# --- Active project ---
# One project per build tree. It supplies the executable, its own _Output/_ProjectFiles/_Saved,
# and the jpt::GetApp() that JPT_SYNC_APP defines for the engine.
set(JUPITER_PROJECT "" CACHE PATH "Active project directory")

# No default. Which project is being built decides the binary, the bundle id and where _Output
# goes, so guessing one would only produce a build nobody asked for.
if(NOT JUPITER_PROJECT)
    message(FATAL_ERROR "JUPITER_PROJECT is unset. Run: py Scripts/setup.py <config> <project>")
elseif(NOT EXISTS "${JUPITER_PROJECT}/CMakeLists.txt")
    message(FATAL_ERROR "No project at '${JUPITER_PROJECT}': a project needs a CMakeLists.txt")
endif()

# The directory's name, which is already the binary's name too -- Scripts/utils.py derives both
# the same way. Here rather than from JPT_SYNC_APP because jpt::GetAppName() is consteval, and an
# immediate function has to be *defined* in every translation unit that calls it: the project's
# definition is exactly what engine code cannot see.
get_filename_component(JUPITER_APP_NAME "${JUPITER_PROJECT}" NAME)
add_compile_definitions(JUPITER_APP_NAME="${JUPITER_APP_NAME}")

# --- Sources ---
file(GLOB_RECURSE JUPITER_SOURCES        CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cpp")
file(GLOB_RECURSE JUPITER_MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cppm")

# Each .mm guards its own body with #if IS_PLATFORM_*, so one glob covers every target.
if(APPLE)
    file(GLOB_RECURSE JUPITER_OBJCXX_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.mm")
endif()

# The editor is a development tool: its panels, and the ImGui they are built on, are absent from a
# binary that has no editor rather than compiled and left unreachable. EditorUI.cpp includes
# imgui.h unguarded, so this is also what keeps it out of a build where ImGui was never compiled.
if(NOT JUPITER_EDITOR)
    list(FILTER JUPITER_SOURCES        EXCLUDE REGEX "/Source/Editor/")
    list(FILTER JUPITER_MODULE_SOURCES EXCLUDE REGEX "/Source/Editor/")
endif()

# No *Tests filter here any more: the suites live in Projects/UnitTests, so the engine has no test
# code left to strip out of a Release build.

# --- Engine library ---
# main() is engine code, but it belongs in the executable's own objects rather than in the archive,
# where pulling it in would depend on the linker resolving the entry point from a member.
set(JUPITER_MAIN_SOURCE "${CMAKE_SOURCE_DIR}/Source/Main.cpp")
list(REMOVE_ITEM JUPITER_SOURCES "${JUPITER_MAIN_SOURCE}")

add_library(JupiterEngine STATIC ${JUPITER_SOURCES} ${JUPITER_OBJCXX_SOURCES})

# PUBLIC, so the project's own sources reach Applications/GetApp.h and Graphics/Renderer.h.
# Everything else stays PRIVATE: for a static library CMake re-exports private link dependencies
# to consumers wrapped in $<LINK_ONLY:>, so the app's link line gets them regardless.
target_include_directories(JupiterEngine PUBLIC "${CMAKE_SOURCE_DIR}/Source")

set_target_properties(JupiterEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_Output/${JUPITER_PRESET}")

target_sources(JupiterEngine
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${JUPITER_MODULE_SOURCES}
)

# Where editor preferences and logs go. Deliberately absent on mobile, whose sandboxes forbid any
# path outside the app container, and in Release, which ships to machines that have no repo --
# GetSavedDir() falls back to the platform's own per-user location whenever this is undefined.
if(NOT JUPITER_CONFIG STREQUAL "RELEASE" AND NOT JUPITER_PLATFORM IN_LIST JUPITER_MOBILE_PLATFORMS)
    target_compile_definitions(JupiterEngine PRIVATE JUPITER_SAVED_DIR="${JUPITER_PROJECT}/_Saved")
endif()

# --- Vendor -- SDL3 ---
# Outside every conditional: SDL is the window, the event loop and the entry point on all four
# platforms and in every config, not a development tool like ImGui.
#
# Static, never shared. Scripts/Build/sign.py runs codesign without --deep, so a dylib nested in
# the bundle would ship unsigned, and Projects/<Name>/CMakeLists.txt gives iOS no rpath to find
# one with.
set(JUPITER_SDL "${CMAKE_SOURCE_DIR}/Vendor/SDL3/Source")
if(NOT EXISTS "${JUPITER_SDL}/include/SDL3/SDL.h")
    message(FATAL_ERROR "SDL3 missing. See Vendor/SDL3/README.md")
endif()

set(SDL_STATIC       ON  CACHE BOOL "" FORCE)
set(SDL_SHARED       OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES     OFF CACHE BOOL "" FORCE)
set(SDL_TESTS        OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL      OFF CACHE BOOL "" FORCE)

# CMAKE_CXX_MODULE_STD initialises CXX_MODULE_STD on every C++ target created after it, so it has
# to be off across the add_subdirectory rather than unset on the targets afterwards.
set(JUPITER_MODULE_STD "${CMAKE_CXX_MODULE_STD}")
set(CMAKE_CXX_MODULE_STD OFF)
add_subdirectory("${JUPITER_SDL}" "${CMAKE_BINARY_DIR}/SDL3" EXCLUDE_FROM_ALL)
set(CMAKE_CXX_MODULE_STD "${JUPITER_MODULE_STD}")

# SYSTEM so SDL's headers never trip -Wpedantic -Werror in our sources; -w so its own C does not
# have to survive a warning set it was never written for.
set_target_properties(SDL3-static PROPERTIES CXX_SCAN_FOR_MODULES OFF SYSTEM TRUE)
target_compile_options(SDL3-static PRIVATE -w)

# PUBLIC, unlike every other dependency: Source/Main.cpp is compiled into the *app* target rather
# than the archive, and it is the translation unit that includes SDL_main.h.
target_link_libraries(JupiterEngine PUBLIC SDL3::SDL3-static)

# --- Apple ---
if(APPLE)
    # Everything the two Apple platforms disagree on, in one place.
    # Device only -- there is no Simulator preset, because its SDK declares no MTL4 protocols.
    if(JUPITER_PLATFORM STREQUAL "IOS")
        set(JUPITER_WINDOW_FRAMEWORK "-framework UIKit")
        set(JUPITER_METAL_SDK        "iphoneos")
        set(JUPITER_METAL_MIN_OS     "-mios-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
        set(JUPITER_INFO_PLIST       "${CMAKE_SOURCE_DIR}/Build/Apple/IOSInfo.plist.in")
    else()
        set(JUPITER_WINDOW_FRAMEWORK "-framework Cocoa")
        set(JUPITER_METAL_SDK        "macosx")
        set(JUPITER_METAL_MIN_OS     "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
        set(JUPITER_INFO_PLIST       "${CMAKE_SOURCE_DIR}/Build/Apple/MacOSInfo.plist.in")
    endif()

    # --- Vendor -- metal-cpp ---
    set(JUPITER_METAL_CPP "${CMAKE_SOURCE_DIR}/Vendor/Metal-Cpp/Include")
    if(NOT EXISTS "${JUPITER_METAL_CPP}/Metal/Metal.hpp")
        message(FATAL_ERROR "metal-cpp missing. See Vendor/Metal-Cpp/README.md")
    endif()
    # SYSTEM so -Wpedantic -Werror does not fire on metal-cpp's own headers, which use
    # anonymous structs. Our sources keep the full warning set.
    target_include_directories(JupiterEngine SYSTEM PRIVATE "${JUPITER_METAL_CPP}")

    # --- Vendor -- Dear ImGui ---
    # Its own library, not loose sources on JupiterEngine, for three reasons: -w, because ImGui
    # does not survive our warning set; CXX_SCAN_FOR_MODULES OFF, so clang-scan-deps never walks
    # vendor code; and a build with no editor skips the target rather than compiling it unreachable.
    if(JUPITER_EDITOR)
        set(JUPITER_IMGUI "${CMAKE_SOURCE_DIR}/Vendor/ImGui/Source")
        if(NOT EXISTS "${JUPITER_IMGUI}/imgui.cpp")
            message(FATAL_ERROR "Dear ImGui missing. See Vendor/ImGui/README.md")
        endif()

        set(JUPITER_IMGUI_SOURCES
            "${JUPITER_IMGUI}/imgui.cpp"
            "${JUPITER_IMGUI}/imgui_draw.cpp"
            "${JUPITER_IMGUI}/imgui_tables.cpp"
            "${JUPITER_IMGUI}/imgui_widgets.cpp"
            "${JUPITER_IMGUI}/imgui_demo.cpp"
            "${JUPITER_IMGUI}/backends/imgui_impl_metal4.mm"
            "${JUPITER_IMGUI}/backends/imgui_impl_sdl3.cpp")

        add_library(JupiterImGui STATIC ${JUPITER_IMGUI_SOURCES})
        set_target_properties(JupiterImGui PROPERTIES CXX_SCAN_FOR_MODULES OFF)
        target_include_directories(JupiterImGui PUBLIC "${JUPITER_IMGUI}" "${JUPITER_IMGUI}/backends")
        target_compile_options(JupiterImGui PRIVATE -w $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)
        target_link_libraries(JupiterImGui PUBLIC SDL3::SDL3-static)

        # SYSTEM on the consuming side too, so imgui.h never trips -Wpedantic -Werror in our code.
        target_include_directories(JupiterEngine SYSTEM PRIVATE "${JUPITER_IMGUI}" "${JUPITER_IMGUI}/backends")
        target_link_libraries(JupiterEngine PRIVATE JupiterImGui)
    endif()

    # --- Frameworks ---
    target_compile_options(JupiterEngine PRIVATE $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)

    # Foundation is required: Homebrew clang does not auto-link libobjc like Apple clang,
    # so without it the link fails on _objc_release.
    # ImageIO decodes the PBR maps; it is a plain C API, so TextureLoader.cpp needs no ObjC++.
    target_link_libraries(JupiterEngine PRIVATE
        "-framework Foundation" "-framework Metal" "-framework QuartzCore"
        "-framework ImageIO" "-framework CoreGraphics"
        "${JUPITER_WINDOW_FRAMEWORK}")

endif()

# --- Project ---
# Last, so the project inherits every directory-scoped define, flag and JUPITER_* variable above.
# The explicit binary dir is what allows a project outside this source tree.
add_subdirectory("${JUPITER_PROJECT}" "${CMAKE_BINARY_DIR}/App")
