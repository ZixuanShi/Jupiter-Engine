# Copyright Jupiter Technologies, Inc. All Rights Reserved.
#
# JupiterEngineClient.cmake
# Reusable helper for client projects that link against JupiterEngine.
#
# Usage in a client CMakeLists.txt:
#
#   include(${JUPITER_ENGINE_ROOT}/Scripts/JupiterEngineClient.cmake)
#   jupiter_add_client(MyGame)
#
# The macro creates an executable target named <project_name>, links it
# against JupiterEngine, and sets up the same defines and flags that the
# old Premake CreateClientProject() function provided.

macro(jupiter_add_client project_name)

    # Collect client source files
    file(GLOB_RECURSE _CLIENT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/**/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/**/*.h"
    )
    file(GLOB_RECURSE _CLIENT_CXX_MODS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/**/*.cppm"
    )

    # On Windows, WIN32 sets /SUBSYSTEM:WINDOWS so WinMain is the entry point.
    # On all other platforms it is ignored.
    if(WIN32)
        add_executable(${project_name} WIN32 ${_CLIENT_SOURCES})
    else()
        add_executable(${project_name} ${_CLIENT_SOURCES})
    endif()

    # Redirect the built .exe to <client_dir>/_Output/<project>_<platform>_<config>_Output/
    # matching premake's targetdir (g_context.project_dir .. "_Output/%{prj.name}_" .. g_outputPath .. "_Output").
    foreach(config Debug Dev Release)
        string(TOUPPER ${config} CONFIG_UPPER)
        set_target_properties(${project_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}
                "${CMAKE_CURRENT_SOURCE_DIR}/_Output/${project_name}_${JPT_PLATFORM}_${config}_Output"
            PDB_OUTPUT_DIRECTORY_${CONFIG_UPPER}
                "${CMAKE_CURRENT_SOURCE_DIR}/_Output/${project_name}_${JPT_PLATFORM}_${config}_Output"
        )
    endforeach()

    # C++20 standard — must be set directly on the target; CMAKE_CXX_STANDARD set
    # inside add_subdirectory() does not propagate back to the parent scope.
    target_compile_features(${project_name} PUBLIC cxx_std_20)

    # Register client module interfaces (replaces Premake "allmodulespublic")
    if(_CLIENT_CXX_MODS)
        target_sources(${project_name} PUBLIC
            FILE_SET CXX_MODULES FILES ${_CLIENT_CXX_MODS}
        )
    endif()

    # Mirror the Source/ directory tree in the VS Solution Explorer,
    # matching the folder structure that Premake generated.
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        FILES ${_CLIENT_SOURCES} ${_CLIENT_CXX_MODS})

    # Link engine (brings in all PUBLIC includes, defines, and libs)
    target_link_libraries(${project_name} PRIVATE Engine)

    # Client-specific include directory (own Source/ folder)
    target_include_directories(${project_name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    )

    # Client defines (replaces Premake JPT_CLIENT_DIR_W and IS_CLIENT defines)
    target_compile_definitions(${project_name} PRIVATE
        "JPT_CLIENT_DIR_W=L\"${CMAKE_CURRENT_SOURCE_DIR}/\""
        IS_CLIENT
    )

    # Static runtime — must match the engine's runtime library
    if(MSVC)
        set_property(TARGET ${project_name} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

        # Redirect intermediate files (.obj) to match premake's objdir pattern:
        # g_context.project_dir .. "_Output/<project>_<platform>_<config>_Intermediate"
        # ${JPT_PLATFORM} is a CMake configure-time string; $(Configuration) is an MSBuild macro.
        set_target_properties(${project_name} PROPERTIES
            VS_INTERMEDIATE_DIRECTORY
                "${CMAKE_CURRENT_SOURCE_DIR}/_Output/${project_name}_${JPT_PLATFORM}_$(Configuration)_Intermediate/")
    endif()

    # Set the client as the VS startup project so pressing F5 runs it,
    # not CMake's ALL_BUILD bookkeeping target.
    set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY
        VS_STARTUP_PROJECT ${project_name})

    # Launch the debugger from the output directory so relative paths
    # (Assets/, _Baked/, Config/) resolve the same way they do at runtime.
    set_target_properties(${project_name} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${project_name}>")

    # Copy runtime data to the output directory for all configurations.
    # The exe lives in _Output/<project>_<config>_Output/ so data must be alongside it.
    # if(EXISTS ...) is evaluated at configure time; all guarded directories exist in the repo.

    # Client Assets  (replaces premake: xcopy "$(SolutionDir)..\Assets" "$(OutDir)Assets")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
        add_custom_command(TARGET ${project_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
                "$<TARGET_FILE_DIR:${project_name}>/Assets"
            COMMENT "Copying client assets"
        )
    endif()

    # Engine common Assets  (replaces premake: xcopy "<engine>/Assets/Jupiter_Common" ...)
    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${JUPITER_ENGINE_ROOT}/Assets/Jupiter_Common"
            "$<TARGET_FILE_DIR:${project_name}>/Assets/Jupiter_Common"
        COMMENT "Copying engine common assets"
    )

    # Client _Baked — Release only (Debug/Dev read from the source tree directly via JPT_ENGINE_DIR_W)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/_Baked")
        add_custom_command(TARGET ${project_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E
                $<IF:$<CONFIG:Release>,copy_directory,true>
                "${CMAKE_CURRENT_SOURCE_DIR}/_Baked"
                "$<TARGET_FILE_DIR:${project_name}>/_Baked"
            COMMENT "Copying client baked data (Release only)"
        )
    endif()

    # Engine common _Baked — Release only (Debug/Dev read from engine source tree directly)
    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E
            $<IF:$<CONFIG:Release>,copy_directory,true>
            "${JUPITER_ENGINE_ROOT}/_Baked/Jupiter_Common"
            "$<TARGET_FILE_DIR:${project_name}>/_Baked/Jupiter_Common"
        COMMENT "Copying engine baked shaders (Release only)"
    )

    # Client Config  (ProjectSettings.json and any other config files)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Config")
        add_custom_command(TARGET ${project_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/Config"
                "$<TARGET_FILE_DIR:${project_name}>/Config"
            COMMENT "Copying client config"
        )
    endif()

    # Clean up internal variables
    unset(_CLIENT_SOURCES)
    unset(_CLIENT_CXX_MODS)

endmacro()
