-- Copyright Jupiter Technologies, Inc. All Rights Reserved.

---------------------------------------------------------------------------------------------------
-- Helper functions
---------------------------------------------------------------------------------------------------
-- @return The root directory of Jupiter Engine
local function GetJupiterDir()
    local currentDir = path.getdirectory(_SCRIPT)

    -- Removes "/Scripts" from the path
    currentDir = currentDir:sub(1, currentDir:find("Scripts") - 1)
    return currentDir
end

---------------------------------------------------------------------------------------------------
-- Data
---------------------------------------------------------------------------------------------------
g_jupiterDir = GetJupiterDir()
g_outputPath = "%{cfg.platform}_%{cfg.buildcfg}"

-- Client can override context variables to modify build settings
g_context = {}
g_context.configurations =
{
    "Debug",   -- Debugging. No optimization will be performed
    "Dev",     -- Development. Use Engine's editors and tools. Also use for profiling
    "Release", -- Release the project. Use for final builds to users. No editors
}
g_context.platforms =
{
    -- Client, Server, Editor
    "Windows_Client",
    "Windows_Editor",
}

---------------------------------------------------------------------------------------------------
-- Jupiter workspace
---------------------------------------------------------------------------------------------------
function CreateEngineWorkspace()
    workspace (g_context.project_name)
        configurations(g_context.configurations)
        platforms(g_context.platforms)
        startproject (g_context.project_name)

        -- Path
        location  (g_context.project_dir .. "_ProjectFiles")
        targetdir (g_context.project_dir .. "_Output/%{prj.name}_" .. g_outputPath .. "_Output")
        objdir    (g_context.project_dir .. "_Output/%{prj.name}_" .. g_outputPath .. "_Intermediate")

        -- Programming
        language   "C++"
        cppdialect "C++20"
        warnings   "Extra"
        staticruntime    "On"
        allmodulespublic "On"

        -- Macro defines
        defines
        {
            ("JPT_ENGINE_DIR_W=L\"" .. g_jupiterDir .."\""),
            ("NOMINMAX"),   -- To get rid of built-in min/max macros
            ("GLFW_STATIC"), -- To use GLFW as a static library
        }

        -- Global filters for configurations
        filter "configurations:Debug"
            defines { "IS_DEBUG" }
            optimize "Off"
            symbols "On"

        filter "configurations:Dev"
            defines { "IS_DEV" }
            optimize "Speed"
            symbols "On"

        filter "configurations:Release"
            defines { "IS_RELEASE" }
            optimize "Speed"
            symbols "off"
            flags{ "FatalCompileWarnings" }

        -- Release shouldn't use editors
        filter { "configurations:Release", "platforms:Windows_Editor" }
            flags { "ExcludeFromBuild" } 

        -- Global filters for windows
        filter "platforms:Windows_Client or Windows_Editor"
            defines { "IS_PLATFORM_WINDOWS" }
            system "Windows"
            architecture "x86_64"

        -- Global filters for clients
        filter "platforms:Windows_Client"
            defines { "IS_CLIENT" }

        -- Global filters for editors
        filter "platforms:Windows_Editor"
            defines { "IS_EDITOR" }

    -- Jupiter Engine
    project "Engine"
        location  (g_jupiterDir .. "_ProjectFiles")
        targetdir (g_jupiterDir .. "_Output/%{prj.name}_" .. g_outputPath .. "_Output")
        objdir    (g_jupiterDir .. "_Output/%{prj.name}_" .. g_outputPath .. "_Intermediate")
        kind "StaticLib"

        includedirs
        {
            (g_jupiterDir .. "Source"),
            (g_jupiterDir .. "Dependencies/*/Include"),
        }
        files 
        {
            (g_jupiterDir .. "Source/**"),
        }
end

function CreateClientProject()
    project (g_context.project_name)
        filter "platforms:Windows_Client or Windows_Editor"
            kind "WindowedApp"

        defines
        {
            ("JPT_CLIENT_DIR_W=L\"" .. g_context.project_dir .."\""),
        }

        includedirs
        {
            (g_jupiterDir .. "Source"),
            (g_jupiterDir .. "Dependencies/*/Include"),
            (g_context.project_dir .. "Source"),
        }
        files
        {
            (g_context.project_dir .. "Source/**"),
        }

        libdirs
        {
            (g_jupiterDir .. "_Output/Engine_" .. g_outputPath .. "_Output"),
            (g_jupiterDir .. "Dependencies/*/Libs"),
        }
        links
        {
            -- Engine's functions, classes, and modules
            "Engine.lib",
            "Engine",

            -- Rendering
            "vulkan-1",
            "d3d12",
            "dxgi",
            "d3dcompiler",
        }

        filter "configurations:Debug"
            links
            {
                "glfw3_Debug",
            }
        filter "configurations:Dev or Release"
            links
            {
                "glfw3_Release",
            }
        filter "configurations:Release"
            postbuildcommands
            {
                -- Assets
                "xcopy \"$(SolutionDir)..\\Assets\"" .. " \"$(OutDir)Assets\"  /e /s /h /i /y",  -- Game Assets
                "xcopy \"" .. g_jupiterDir .."Assets\\Jupiter_Common\"" .. " \"$(OutDir)Assets\\Jupiter_Common\"  /e /s /h /i /y",    -- Engine Common Assets

                -- _Baked
                "xcopy \"$(SolutionDir)..\\_Baked\"" .. " \"$(OutDir)_Baked\"  /e /s /h /i /y",
                "xcopy \"" .. g_jupiterDir .."_Baked\\Jupiter_Common\"" .. " \"$(OutDir)_Baked\\Jupiter_Common\"  /e /s /h /i /y",
            }
end

function ParseContext()
    -- Parse command line arguments to context
    for i = 1, #_ARGS do
        -- arg is either key-value paired like "platform=win64_Client" or a flag like "show_fps"
        arg = _ARGS[i]

        -- Key-value paired arguments
        if arg:find("=") then
            local key, value = arg:match("([^=]+)=([^=]+)")
            g_context[key] = value
        else
            g_context[arg] = true
        end
    end

    assert(g_context.project_name, "Project name is not provided")
    assert(g_context.project_dir, "Project directory is not provided")

    -- Log context variables
    for key, value in pairs(g_context) do
        local valueStr = ""

        if type(value) == "table" then
            valueStr = valueStr .. "{ "
            for _, v in pairs(value) do
                valueStr = valueStr .. v .. ", "
            end
            valueStr = valueStr .. "}"
        else
            valueStr = value
        end

        print(key, valueStr)
    end
end

function GenerateProjectFiles()
    print("----------------------------------------------------\n-- Parsing command line arguments\n----------------------------------------------------")
    ParseContext()

    print("----------------------------------------------------\n-- Generating Project Files\n----------------------------------------------------")
    CreateEngineWorkspace()
    CreateClientProject()
end