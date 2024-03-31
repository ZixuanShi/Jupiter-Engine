-- Client project should have a Generate.bat file that should be like this:

-- cd "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"
-- set args="<ProjectName>" "<ProjectDirectory>"
-- call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\_Bin\premake5.exe" vs2022 %args%

-- Example:
-- cd "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"
-- set args="UnitTests" "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/UnitTests/"
-- call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\_Bin\premake5.exe" vs2022 %args%

local project_name = _ARGS[1]
local project_dir  = _ARGS[2]
local jupiter_dir = "C:/Program Files/Jupiter Technologies/Jupiter-Engine/"   -- Needs to be changed if the Jupiter-Engine or Premake5.exe moves.
local output_path = "%{cfg.platform}_%{cfg.buildcfg}"

for i = 1, #_ARGS do
    print("Argument " .. i .. " = " .. _ARGS[i])
end

function HasArg(name)
    for i = 1, #_ARGS do
        if _ARGS[i] == name then
            return true
        end
    end
    return false
end

-- Jupiter workspace
workspace (project_name)
    configurations 
    { 
        "Debug",        -- Debugging. No optimization will be performed
        "Development",  -- Develop the project. Use Engine's editors and tools 
        "Profiling",    -- Profiling. Same configurations with Release but enables Profiling code
        "Release",      -- Relese/Shipping
    }

    platforms 
    {
        "Win64",
    }

    startproject (project_name)

    -- Paths
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
        ("JPT_ENGINE_DIR=\""    .. jupiter_dir .."\""),
        ("JPT_ENGINE_DIR_W=L\"" .. jupiter_dir .."\""),
        ("NOMINMAX"),   -- To get rid of built-in min/max macros
    }

    -- Global filters for configurations
    filter "configurations:Release or configurations:Profiling"
        flags{ "FatalCompileWarnings" }

    filter "configurations:Debug"
        defines 
        { 
            "IS_DEBUG",
        }
        buildoptions { "/MTd" }
        optimize "Off"
        symbols "On"

    filter "configurations:Development"
        defines 
        { 
            "IS_DEVELOPMENT",
        }
        buildoptions { "/MT" }
        optimize "Size"
        symbols "On"

    filter "configurations:Profiling"
        defines 
        { 
            "IS_PROFILING",
        }
        buildoptions { "/MT" }
        optimize "Speed"
        symbols "off"

    filter "configurations:Release"
        defines 
        { 
            "IS_RELEASE",
        }
        buildoptions { "/MT" }
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
    }

    defines
    {
        ("IS_CLIENT=0"),
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

    includedirs
    {
        (jupiter_dir .. "Source"),
        (project_dir .. "Source")
    }

    links
    {
        -- Engine's functions, classes, and modules
        "Engine.lib",
        "Engine",

        -- DirectX 12
        "d3d12",
        "dxgi",
        "d3dcompiler",
    }

    defines
    {
        ("JPT_CLIENT_DIR=\""    .. project_dir .."\""),
        ("JPT_CLIENT_DIR_W=L\"" .. project_dir .."\""),
        ("IS_CLIENT=1"),
    }

    libdirs
    {
        (jupiter_dir .. "_Bin/Engine_" .. output_path .. "_Output/")
    }

    files
    {
        (project_dir .. "Source/**.h"),
        (project_dir .. "Source/**.cpp"),
        (project_dir .. "Source/**.ixx"),
    }