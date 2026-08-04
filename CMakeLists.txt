cmake_minimum_required(VERSION 3.30)

# --- C++ standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

# --- Apple ---
# Must precede project(): CMAKE_CXX_STDLIB_MODULES_JSON is only read during compiler detection.
if(APPLE)
    get_filename_component(_cxx_bin  "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_cxx_root "${_cxx_bin}" DIRECTORY)

    if(EXISTS "${_cxx_root}/lib/c++/libc++.modules.json")
        set(CMAKE_CXX_STDLIB_MODULES_JSON "${_cxx_root}/lib/c++/libc++.modules.json")
    endif()

    # Each .mm guards its own body with #if IS_PLATFORM_*, so one glob covers every target.
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

# --- Sources ---
file(GLOB_RECURSE JUPITER_SOURCES        CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cpp")
file(GLOB_RECURSE JUPITER_MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.cppm")

# The editor is a development tool: its panels, and the ImGui they are built on, are absent from
# a Release binary entirely rather than compiled and left unreachable.
#
# *Tests.cppm goes the same way. Their call sites are already #if !IS_CONFIG_RELEASE, but the
# modules were still compiled and linked -- nm found jpt::RunMathTests in the Release binary -- and
# InputTests pulls the whole Carbon umbrella in with it.
if(JUPITER_CONFIG STREQUAL "RELEASE")
    list(FILTER JUPITER_SOURCES        EXCLUDE REGEX "/Source/Editor/|Tests\\.cpp$")
    list(FILTER JUPITER_MODULE_SOURCES EXCLUDE REGEX "/Source/Editor/|Tests\\.cppm$")
endif()


# --- Executable ---
add_executable(JupiterEngine ${JUPITER_SOURCES} ${JUPITER_OBJCXX_SOURCES})

target_include_directories(JupiterEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source")

# Where editor preferences and logs go. Deliberately absent on iOS, whose sandbox forbids any
# path outside the app container, and in Release, which ships to machines that have no repo --
# GetSavedDir() falls back to the platform's own per-user location whenever this is undefined.
if(NOT JUPITER_CONFIG STREQUAL "RELEASE" AND NOT JUPITER_PLATFORM STREQUAL "IOS")
    target_compile_definitions(JupiterEngine PRIVATE JUPITER_SAVED_DIR="${CMAKE_SOURCE_DIR}/_Saved")
endif()

target_sources(JupiterEngine
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${JUPITER_MODULE_SOURCES}
)

# --- Assets ---
# Everything under Assets/ ships beside the binary in the layout it has in the repo, so one
# relative string means the same thing to jpt::Path here and at run time. Only the destination
# is platform-dependent, and it is the build-side mirror of jpt::GetRoot().
if(JUPITER_PLATFORM STREQUAL "MACOS")
    set(JUPITER_ASSET_DIR "$<TARGET_BUNDLE_CONTENT_DIR:JupiterEngine>/Resources/Assets")
elseif(JUPITER_PLATFORM STREQUAL "IOS")
    set(JUPITER_ASSET_DIR "$<TARGET_BUNDLE_CONTENT_DIR:JupiterEngine>/Assets")
else()
    set(JUPITER_ASSET_DIR "$<TARGET_FILE_DIR:JupiterEngine>/Assets")
endif()

# Staged by a script, not target_sources: routing content through the latter invites CMake to
# interpret it, and a Wavefront .obj gets EXTERNAL_OBJECT stamped on by extension alone and
# lands on the link line. `cmake -E copy_directory` avoids that but can exclude nothing, and
# shader sources and .DS_Store must not reach a signed bundle. A target rather than a POST_BUILD
# rule, so staging precedes the executable's own post-build steps.
add_custom_target(JupiterAssets
    COMMAND python3 "${CMAKE_SOURCE_DIR}/Scripts/stage_assets.py" "${JUPITER_ASSET_DIR}"
    COMMENT "Staging Assets")

add_dependencies(JupiterEngine JupiterAssets)

# --- Apple ---
if(APPLE)
    # Vendor -- metal-cpp
    set(JUPITER_METAL_CPP "${CMAKE_SOURCE_DIR}/Vendor/Metal-Cpp/Include")
    if(NOT EXISTS "${JUPITER_METAL_CPP}/Metal/Metal.hpp")
        message(FATAL_ERROR "metal-cpp missing. See Vendor/Metal-Cpp/README.md")
    endif()
    # SYSTEM so -Wpedantic -Werror does not fire on metal-cpp's own headers, which use
    # anonymous structs. Our sources keep the full warning set.
    target_include_directories(JupiterEngine SYSTEM PRIVATE "${JUPITER_METAL_CPP}")

    # Vendor -- Dear ImGui
    # Its own library, not loose sources on JupiterEngine, for three reasons: -w, because ImGui
    # does not survive our warning set; CXX_SCAN_FOR_MODULES OFF, so clang-scan-deps never walks
    # vendor code; and Release skips the whole target rather than compiling it unreachable.
    if(NOT JUPITER_CONFIG STREQUAL "RELEASE")
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
            "${JUPITER_IMGUI}/backends/imgui_impl_metal4.mm")

        # imgui_impl_osx.mm is the AppKit *input* backend and is macOS-only. The Metal backend
        # above is plain Metal and builds for both; iOS input is hand-written in IOSWindow.mm.
        if(NOT JUPITER_PLATFORM STREQUAL "IOS")
            list(APPEND JUPITER_IMGUI_SOURCES "${JUPITER_IMGUI}/backends/imgui_impl_osx.mm")
        endif()

        add_library(JupiterImGui STATIC ${JUPITER_IMGUI_SOURCES})
        set_target_properties(JupiterImGui PROPERTIES CXX_SCAN_FOR_MODULES OFF)
        target_include_directories(JupiterImGui PUBLIC "${JUPITER_IMGUI}" "${JUPITER_IMGUI}/backends")
        target_compile_options(JupiterImGui PRIVATE -w $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)

        # GameController is imgui_impl_osx's own dependency -- it reads GCController for gamepad
        # navigation -- so it belongs on this target rather than on the engine's framework list.
        if(NOT JUPITER_PLATFORM STREQUAL "IOS")
            target_link_libraries(JupiterImGui PUBLIC "-framework GameController")
        endif()

        # SYSTEM on the consuming side too, so imgui.h never trips -Wpedantic -Werror in our code.
        target_include_directories(JupiterEngine SYSTEM PRIVATE "${JUPITER_IMGUI}" "${JUPITER_IMGUI}/backends")
        target_link_libraries(JupiterEngine PRIVATE JupiterImGui)
    endif()

    # ARC
    target_compile_options(JupiterEngine PRIVATE $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)

    # Frameworks
    # Foundation is required: Homebrew clang does not auto-link libobjc like Apple clang,
    # so without it the link fails on _objc_release.
    # ImageIO decodes the PBR maps; it is a plain C API, so TextureLoader.cpp needs no ObjC++.
    target_link_libraries(JupiterEngine PRIVATE
        "-framework Foundation" "-framework Metal" "-framework QuartzCore"
        "-framework ImageIO" "-framework CoreGraphics")

    # Windowing is the only framework the two Apple platforms disagree on.
    if(JUPITER_PLATFORM STREQUAL "IOS")
        target_link_libraries(JupiterEngine PRIVATE "-framework UIKit")
    else()
        target_link_libraries(JupiterEngine PRIVATE "-framework Cocoa")
    endif()

    # App bundle
    # iOS will not install a non-bundle. On macOS it supplies the CFBundleIdentifier, whose
    # absence makes the system log registration failures on every launch.
    set(MACOSX_BUNDLE_EXECUTABLE_NAME      "JupiterEngine")
    set(MACOSX_BUNDLE_BUNDLE_NAME          "Jupiter Engine")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER       "com.jupitertechnologies.engine")
    set(MACOSX_BUNDLE_BUNDLE_VERSION       "1")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")

    # Device only -- there is no Simulator preset, because its SDK declares no MTL4 protocols.
    if(JUPITER_PLATFORM STREQUAL "IOS")
        set(JUPITER_METAL_SDK "iphoneos")
        set(JUPITER_METAL_MIN_OS "-mios-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
        set(JUPITER_INFO_PLIST "${CMAKE_SOURCE_DIR}/Build/Apple/IOSInfo.plist.in")
    else()
        set(JUPITER_METAL_SDK "macosx")
        set(JUPITER_METAL_MIN_OS "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
        set(JUPITER_INFO_PLIST "${CMAKE_SOURCE_DIR}/Build/Apple/MacOSInfo.plist.in")
    endif()

    set_target_properties(JupiterEngine PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${JUPITER_INFO_PLIST}")

    # Shaders
    # Every .metal file links into one default.metallib, so adding a shader needs no edit here.
    # Note modern Xcode ships no separate `metallib` tool -- `metal` links .air files itself.
    #
    # The version-min flag is not optional. Unlike clang, `metal` ignores CMAKE_OSX_DEPLOYMENT_TARGET
    # and targets the SDK version, stamping the library with an AIR version the deployment target
    # never promised. Loading it on an older OS then fails as a *nil* newDefaultLibrary() rather
    # than an error, so the app runs on with a dead renderer.
    file(GLOB JUPITER_SHADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Assets/Shaders/*.metal")

    set(JUPITER_AIR_FILES "")
    foreach(_shader ${JUPITER_SHADERS})
        get_filename_component(_shader_name "${_shader}" NAME_WE)
        set(_air "${CMAKE_CURRENT_BINARY_DIR}/Shaders/${_shader_name}.air")

        # The depfile is not optional either. ShaderTypes.h is compiled by clang *and* by metal,
        # so without it a change to the uniform struct rebuilds only the C++ half and the shader
        # goes on reading the old layout -- no compile error, and the static_assert that exists to
        # catch exactly this never re-evaluates. -MMD over -MD so an Xcode update does not
        # invalidate every shader through <metal_stdlib>.
        add_custom_command(OUTPUT "${_air}"
            COMMAND xcrun -sdk ${JUPITER_METAL_SDK} metal ${JUPITER_METAL_MIN_OS}
                    -I "${CMAKE_SOURCE_DIR}/Source" -MMD -MF "${_air}.d" -c "${_shader}" -o "${_air}"
            DEPENDS "${_shader}"
            DEPFILE "${_air}.d"
            COMMENT "Compiling ${_shader_name}.metal")

        list(APPEND JUPITER_AIR_FILES "${_air}")
    endforeach()

    set(JUPITER_METALLIB "${CMAKE_CURRENT_BINARY_DIR}/Shaders/default.metallib")
    add_custom_command(OUTPUT "${JUPITER_METALLIB}"
        COMMAND xcrun -sdk ${JUPITER_METAL_SDK} metal ${JUPITER_METAL_MIN_OS} ${JUPITER_AIR_FILES} -o "${JUPITER_METALLIB}"
        DEPENDS ${JUPITER_AIR_FILES}
        COMMENT "Linking default.metallib")

    # Where newDefaultLibrary() looks. CMake resolves "Resources" to Contents/Resources on
    # macOS and to the bundle root on iOS, which is what each platform expects.
    target_sources(JupiterEngine PRIVATE "${JUPITER_METALLIB}")
    set_source_files_properties("${JUPITER_METALLIB}" PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
        GENERATED TRUE)

    # Code signing
    # A device refuses to launch an app without an embedded provisioning profile naming it.
    # The simulator has no such check, so its bundles stay unsigned.
    #
    # Its own target rather than a POST_BUILD rule on the executable, because a signature seals
    # the staged assets too and those change without the executable ever relinking -- which used
    # to leave the bundle sealed against a file it no longer held, and a device rejects that.
    # Signing takes ~0.1s, so it runs every build rather than guessing when it is needed.
    if(JUPITER_PLATFORM STREQUAL "IOS")
        add_custom_target(JupiterSign ALL
            COMMAND python3 "${CMAKE_SOURCE_DIR}/Scripts/sign.py" "$<TARGET_BUNDLE_DIR:JupiterEngine>"
            COMMENT "Code-signing for device")

        add_dependencies(JupiterSign JupiterEngine JupiterAssets)
    endif()

    # rpath
    # iOS resolves libc++ from /usr/lib, so a Homebrew rpath would only be dead weight.
    if(NOT JUPITER_PLATFORM STREQUAL "IOS" AND EXISTS "${_cxx_root}/lib/c++/libc++.dylib")
        set_target_properties(JupiterEngine PROPERTIES
            BUILD_RPATH "${_cxx_root}/lib/c++"
            INSTALL_RPATH "${_cxx_root}/lib/c++")
    endif()
endif()
