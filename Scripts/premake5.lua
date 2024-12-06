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
jupiter_dir = GetJupiterDir()
output_path = "%{cfg.platform}_%{cfg.buildcfg}"

-- Client can override context variables to modify build settings
context = {}
context.configurations =
{
    -- Debugging. No optimization will be performed
    "Debug",
    "Debug Editor",

    -- Develop the project. Use Engine's editors and tools 
    "Development",
    "Development Editor",

    -- Relese/Shipping
    "Release",
}
context.platforms =
{
    "Win64",
}

---------------------------------------------------------------------------------------------------
-- Jupiter workspace
---------------------------------------------------------------------------------------------------
function CreateEngineWorkspace()
    workspace (context.project_name)
        configurations(context.configurations)
        platforms(context.platforms)
        startproject (context.project_name)

        -- Path
        location  (context.project_dir .. "_ProjectFiles")
        targetdir (context.project_dir .. "_Output/%{prj.name}_" .. output_path .. "_Output")
        objdir    (context.project_dir .. "_Output/%{prj.name}_" .. output_path .. "_Intermediate")

        -- Programming
        language   "C++"
        cppdialect "C++20"
        warnings   "Extra"
        staticruntime    "On"
        allmodulespublic "On"

        -- Macro defines
        defines
        {
            ("JPT_ENGINE_DIR_W=L\"" .. jupiter_dir .."\""),
            ("NOMINMAX"),   -- To get rid of built-in min/max macros
            ("GLFW_STATIC"), -- To use GLFW as a static library
        }

        -- Global filters for configurations
        filter "configurations:Release"
            flags{ "FatalCompileWarnings" }

        filter "configurations:Debug or Debug Editor"
            defines { "IS_DEBUG" }
            optimize "Off"
            symbols "On"

        filter "configurations:Development or Development Editor"
            defines { "IS_DEVELOPMENT" }
            optimize "Speed"
            symbols "On"

        filter "configurations:Release"
            defines { "IS_RELEASE" }
            optimize "Speed"
            symbols "off"

        filter "configurations:Debug Editor or Development Editor"
            defines { "IS_EDITOR" }

        -- Global filters for platforms
        filter "platforms:Win64"
            defines { "IS_PLATFORM_WIN64" }

        -- Only windows platform supports editor
        filter "platforms:not Win64"
            removedefines { "IS_EDITOR" }

    -- Jupiter Engine
    project "Engine"
        location  (jupiter_dir .. "_ProjectFiles")
        targetdir (jupiter_dir .. "_Output/%{prj.name}_" .. output_path .. "_Output")
        objdir    (jupiter_dir .. "_Output/%{prj.name}_" .. output_path .. "_Intermediate")
        kind "StaticLib"

        includedirs
        {
            (jupiter_dir .. "Source"),
            (jupiter_dir .. "Dependencies/*/Include"),
        }
        files 
        {
            (jupiter_dir .. "Source/**.h"),
            (jupiter_dir .. "Source/**.cpp"),
            (jupiter_dir .. "Source/**.c"),
            (jupiter_dir .. "Source/**.ixx"),
            (jupiter_dir .. "Source/**.natvis"),
        }
end

function CreateClientProject()
    project (context.project_name)
        filter "platforms:Win64"
            kind "WindowedApp"

        defines
        {
            ("JPT_CLIENT_DIR_W=L\"" .. context.project_dir .."\""),
            ("JPT_OUTPUT_DIR_W=L\"" .. context.project_dir .. "_Output/%{prj.name}_" .. output_path .. "_Output/\""),
        }

        includedirs
        {
            (jupiter_dir .. "Source"),
            (jupiter_dir .. "Dependencies/*/Include"),
            (context.project_dir .. "Source"),
        }
        files
        {
            (context.project_dir .. "Source/**.h"),
            (context.project_dir .. "Source/**.cpp"),
            (context.project_dir .. "Source/**.ixx"),
        }

        libdirs
        {
            (jupiter_dir .. "_Output/Engine_" .. output_path .. "_Output"),
            (jupiter_dir .. "Dependencies/*/Libs"),
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

        filter "configurations:Debug or Debug Editor"
            links
            {
                "glfw3_Debug",
            }
        filter "configurations:Development or Development Editor or Release"
            links
            {
                "glfw3_Release",
            }
        filter "configurations:Release"
            postbuildcommands
            {
                -- Assets
                "xcopy \"$(SolutionDir)..\\Assets\"" .. " \"$(OutDir)Assets\"  /e /s /h /i /y",  -- Game Assets
                "xcopy \"" .. jupiter_dir .."Assets\\Jupiter_Common\"" .. " \"$(OutDir)Assets\\Jupiter_Common\"  /e /s /h /i /y",    -- Engine Common Assets

                -- _Baked
                "xcopy \"$(SolutionDir)..\\_Baked\"" .. " \"$(OutDir)_Baked\"  /e /s /h /i /y",
                "xcopy \"" .. jupiter_dir .."_Baked\\Jupiter_Common\"" .. " \"$(OutDir)_Baked\\Jupiter_Common\"  /e /s /h /i /y",
            }
end

function ParseContext()
    -- Parse command line arguments to context
    for i = 1, #_ARGS do
        -- arg is either key-value paired like "platform=win64" or a flag like "show_fps"
        arg = _ARGS[i]

        -- Key-value paired arguments
        if arg:find("=") then
            local key, value = arg:match("([^=]+)=([^=]+)")
            context[key] = value
        else
            context[arg] = true
        end
    end

    assert(context.project_name, "Project name is not provided")
    assert(context.project_dir, "Project directory is not provided")

    -- Log context variables
    for key, value in pairs(context) do
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