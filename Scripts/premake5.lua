for i = 1, #_ARGS do
    print("Argument " .. i .. " = " .. _ARGS[i])
end

---------------------------------------------------------------------------------------------------
-- Helper functions
---------------------------------------------------------------------------------------------------
-- @return True if the argument is found in the command line arguments
function HasArg(name)
    for i = 1, #_ARGS do
        if _ARGS[i] == name then
            return true
        end
    end
    return false
end

-- @return The root directory of Jupiter Engine
local function GetJupiterRootDir()
    local currentDir = path.getdirectory(_SCRIPT)

    -- Removes "/Scripts" from the path
    currentDir = currentDir:sub(1, currentDir:find("Scripts") - 1)
    return currentDir
end

---------------------------------------------------------------------------------------------------
-- Data
---------------------------------------------------------------------------------------------------
jupiter_dir = GetJupiterRootDir()
output_path = "%{cfg.platform}_%{cfg.buildcfg}"

-- Client can override context variables to modify build settings
context = {}

---------------------------------------------------------------------------------------------------
-- Jupiter workspace
---------------------------------------------------------------------------------------------------
function CreateEngineWorkspace(context)
    workspace (context.project_name)
        configurations 
        { 
            "Debug",        -- Debugging. No optimization will be performed
            "Development",  -- Develop the project. Use Engine's editors and tools 
            "Release",      -- Relese/Shipping
        }

        platforms 
        {
            "Win64",
        }

        startproject (context.project_name)

        -- Path
        location  (context.project_dir .. "_ProjectFiles")
        targetdir (context.project_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Output")
        objdir    (context.project_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Intermediate")

        -- Programming
        language   "C++"
        cppdialect "C++latest"
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

        filter "configurations:Debug"
            defines { "IS_DEBUG" }
            optimize "Off"
            symbols "On"

        filter "configurations:Development"
            defines { "IS_DEVELOPMENT" }
            optimize "Speed"
            symbols "On"

        filter "configurations:Release"
            defines { "IS_RELEASE" }
            optimize "Speed"
            symbols "off"

        -- Global filters for platforms
        filter "platforms:Win64"
            defines { "IS_PLATFORM_WIN64" }

    -- Jupiter Engine
    project "Engine"
        location  (jupiter_dir .. "_ProjectFiles")
        targetdir (jupiter_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Output")
        objdir    (jupiter_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Intermediate")
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
        }
end

-- Default template for clients
function CreateClientProject(context)
    project (context.project_name)
    filter "platforms:Win64"
        kind "WindowedApp"

    defines
    {
        ("JPT_CLIENT_DIR_W=L\"" .. context.project_dir .."\""),
        ("JPT_OUTPUT_DIR_W=L\"" .. context.project_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Output/\""),
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
        (jupiter_dir .. "_Bin/Engine_" .. output_path .. "_Output"),
        (jupiter_dir .. "Dependencies/*/Libs"),
    }
    links
    {
        -- Engine's functions, classes, and modules
        "Engine.lib",
        "Engine",

        -- OpenGL
        "opengl32",

        -- DirectX 12
        "d3d12",
        "dxgi",
        "d3dcompiler",
    }

    filter "configurations:Debug"
        links
        {
            "glfw3_Debug",
        }
    filter "configurations:not Debug"
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
        }
end