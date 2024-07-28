-- Client project should have a Generate.bat file that should be like this:

-- cd /d "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"
-- set args="<ProjectName>" "<ProjectDirectory>"
-- call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Bin\premake5.exe" vs2022 %args%

-- Example:
-- cd /d "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"
-- set args="UnitTests" "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/UnitTests/"
-- call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Bin\premake5.exe" vs2022 %args%


for i = 1, #_ARGS do
    print("Argument " .. i .. " = " .. _ARGS[i])
end

---------------------------------------------------------------------------------------------------
-- Helper functions
---------------------------------------------------------------------------------------------------
function HasArg(name)
    for i = 1, #_ARGS do
        if _ARGS[i] == name then
            return true
        end
    end
    return false
end

function FindJupiterRootDir()
    local path = debug.getinfo(1).source
    local index = path:lower():find("jupiter%-engine")
    path = path:sub(2, index + #"Jupiter-Engine")   -- C:/Program Files/Jupiter Technologies/Jupiter-Engine/Scripts/premake5.lua
    return path
end

---------------------------------------------------------------------------------------------------
-- local data
---------------------------------------------------------------------------------------------------
local project_name = _ARGS[1]
local project_dir  = _ARGS[2]
local jupiter_dir = FindJupiterRootDir()
local output_path = "%{cfg.platform}_%{cfg.buildcfg}"

---------------------------------------------------------------------------------------------------
-- Jupiter workspace
---------------------------------------------------------------------------------------------------
workspace (project_name)
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

    startproject (project_name)

    -- Path
    location  (project_dir .. "_ProjectFiles")
    targetdir (project_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Output")
    objdir    (project_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Intermediate")

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
        (jupiter_dir .. "Source/**.ixx"),
    }

-- Client Project
project (project_name)
    filter "platforms:Win64"
        kind "WindowedApp"

    defines
    {
        ("JPT_CLIENT_DIR_W=L\"" .. project_dir .."\""),
        ("JPT_OUTPUT_DIR_W=L\"" .. project_dir .. "_Bin/%{prj.name}_" .. output_path .. "_Output/\""),
    }

    includedirs
    {
        (jupiter_dir .. "Source"),
        (jupiter_dir .. "Dependencies/*/Include"),
        (project_dir .. "Source"),
    }    
    files
    {
        (project_dir .. "Source/**.h"),
        (project_dir .. "Source/**.cpp"),
        (project_dir .. "Source/**.ixx"),
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
