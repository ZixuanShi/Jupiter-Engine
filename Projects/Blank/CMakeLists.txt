# The executable. Everything above this in the tree is the engine; everything here is the app.
# Scripts/utils.py derives the binary name from this directory's name and the bundle id from
# that name lowercased, so the two below are the same convention spelled once more for CMake.
set(BLANK_NAME      "Blank")
set(BLANK_BUNDLE_ID "com.jupitertechnologies.blank")

file(GLOB_RECURSE BLANK_SOURCES        CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

# Empty today -- ApplicationBlank is plain because JPT_SYNC_APP cannot expand in a module unit --
# but the rest of a project's code is free to be modules, and they need no CMake edit to appear.
file(GLOB_RECURSE BLANK_MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cppm")

add_executable(${BLANK_NAME} ${BLANK_SOURCES} ${JUPITER_MAIN_SOURCE})

target_sources(${BLANK_NAME}
    PRIVATE
        FILE_SET CXX_MODULES
        FILES
            ${BLANK_MODULE_SOURCES}
)

target_link_libraries(${BLANK_NAME} PRIVATE JupiterEngine)

set_target_properties(${BLANK_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/_Output/${JUPITER_PRESET}")

# --- Assets ---
# Everything under the engine's Assets/ ships beside the binary in the layout it has in the repo,
# so one relative string means the same thing to jpt::Path here and at run time. Only the
# destination is platform-dependent, and it is the build-side mirror of jpt::GetRoot().
if(JUPITER_PLATFORM STREQUAL "MACOS")
    set(JUPITER_ASSET_DIR "$<TARGET_BUNDLE_CONTENT_DIR:${BLANK_NAME}>/Resources/Assets")
elseif(JUPITER_PLATFORM STREQUAL "IOS")
    set(JUPITER_ASSET_DIR "$<TARGET_BUNDLE_CONTENT_DIR:${BLANK_NAME}>/Assets")
else()
    set(JUPITER_ASSET_DIR "$<TARGET_FILE_DIR:${BLANK_NAME}>/Assets")
endif()

# Staged by a script, not target_sources: routing content through the latter invites CMake to
# interpret it, and a Wavefront .obj gets EXTERNAL_OBJECT stamped on by extension alone and
# lands on the link line. `cmake -E copy_directory` avoids that but can exclude nothing, and
# shader sources and .DS_Store must not reach a signed bundle. A target rather than a POST_BUILD
# rule, so staging precedes the executable's own post-build steps.
add_custom_target(JupiterAssets
    COMMAND python3 "${CMAKE_SOURCE_DIR}/Scripts/Assets/stage_assets.py" "${JUPITER_ASSET_DIR}"
    COMMENT "Staging Assets")

add_dependencies(${BLANK_NAME} JupiterAssets)

# --- Apple ---
if(APPLE)
    # --- App bundle ---
    # iOS will not install a non-bundle. On macOS it supplies the CFBundleIdentifier, whose
    # absence makes the system log registration failures on every launch.
    set(MACOSX_BUNDLE_EXECUTABLE_NAME      "${BLANK_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_NAME          "${BLANK_NAME}")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER       "${BLANK_BUNDLE_ID}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION       "1")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")

    set_target_properties(${BLANK_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${JUPITER_INFO_PLIST}")

    # --- Shaders ---
    # Every .metal file links into one default.metallib, so adding a shader needs no edit here.
    # Note modern Xcode ships no separate `metallib` tool -- `metal` links .air files itself.
    #
    # Here rather than with the engine because MACOSX_PACKAGE_LOCATION only works on a bundle,
    # and an add_custom_command output must be consumed in the directory that declares it.
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
    target_sources(${BLANK_NAME} PRIVATE "${JUPITER_METALLIB}")
    set_source_files_properties("${JUPITER_METALLIB}" PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
        GENERATED TRUE)

    # --- Code signing ---
    # A device refuses to launch an app without an embedded provisioning profile naming it.
    # The simulator has no such check, so its bundles stay unsigned.
    #
    # Its own target rather than a POST_BUILD rule on the executable, because a signature seals
    # the staged assets too and those change without the executable ever relinking -- which used
    # to leave the bundle sealed against a file it no longer held, and a device rejects that.
    # Signing takes ~0.1s, so it runs every build rather than guessing when it is needed.
    if(JUPITER_PLATFORM STREQUAL "IOS")
        add_custom_target(JupiterSign ALL
            COMMAND python3 "${CMAKE_SOURCE_DIR}/Scripts/Build/sign.py" "$<TARGET_BUNDLE_DIR:${BLANK_NAME}>"
            COMMENT "Code-signing for device")

        add_dependencies(JupiterSign ${BLANK_NAME} JupiterAssets)
    endif()

    # --- rpath ---
    # iOS resolves libc++ from /usr/lib, so a Homebrew rpath would only be dead weight.
    if(NOT JUPITER_PLATFORM STREQUAL "IOS" AND EXISTS "${_cxx_root}/lib/c++/libc++.dylib")
        set_target_properties(${BLANK_NAME} PROPERTIES
            BUILD_RPATH "${_cxx_root}/lib/c++"
            INSTALL_RPATH "${_cxx_root}/lib/c++")
    endif()
endif()
