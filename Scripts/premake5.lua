-- Client project should have a Generate.bat file that should be like this:

-- cd "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"
-- call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Generated\premake5.exe" vs2022 "<ProjectName>" "<ProjectDirectory>"

-- Example:
-- cd "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"
-- call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Generated\premake5.exe" vs2022 "UnitTests" "C:/Program Files/Jupiter Technologies/Jupiter-Engine/Projects/UnitTests/"

local project_name = _ARGS[1]
local project_dir  = _ARGS[2]

-- Jupiter workspace
workspace (project_name)
    -- Globals for jupiter workspace
    local jupiter_outputpath = "%{cfg.platform}_%{cfg.buildcfg}"
    local jupiter_dir = "C:/Program Files/Jupiter Technologies/Jupiter-Engine/"   -- Needs to be changed if the Jupiter-Engine or Premake5.exe moves.

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
    location  (project_dir .. "Visual Studio Files")
    targetdir (project_dir .. "Generated/%{prj.name}_" .. jupiter_outputpath .. "_Output")
    objdir    (project_dir .. "Generated/%{prj.name}_" .. jupiter_outputpath .. "_Intermediate")

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
        ("JPT_PROJECT_DIR=\""   .. project_dir .."\""),
        ("JPT_PROJECT_DIR_W=L\"".. project_dir .."\""),
        ("NOMINMAX"),   -- To get rid of built-in min/max macros
    }

    -- Global filters for configurations
    filter "configurations:Release or configurations:Profiling"
        flags{ "FatalCompileWarnings" }

    filter "configurations:Debug"
        defines 
        { 
            "IS_DEBUG",
            "JPT_ENABLE_EDITOR_FEATURES",
        }
        buildoptions { "/MTd" }
        optimize "Off"
        symbols "On"

    filter "configurations:Development"
        defines 
        { 
            "IS_DEVELOPMENT",
            "JPT_ENABLE_EDITOR_FEATURES",
        }
        buildoptions { "/MT" }
        optimize "Full"
        symbols "On"

    filter "configurations:Profiling"
        defines 
        { 
            "IS_PROFILING",
            "JPT_ENABLE_EDITOR_FEATURES",
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
    location  (jupiter_dir .. "Visual Studio Files")
    targetdir (jupiter_dir .. "Generated/%{prj.name}_" .. jupiter_outputpath .. "_Output")
    objdir    (jupiter_dir .. "Generated/%{prj.name}_" .. jupiter_outputpath .. "_Intermediate")
    kind "StaticLib"

    includedirs
    {
        (jupiter_dir .. "Source"),
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

    libdirs
    {
        (jupiter_dir .. "Generated/Engine_" .. jupiter_outputpath .. "_Output/")
    }

    files
    {
        (project_dir .. "Source/**.h"),
        (project_dir .. "Source/**.cpp"),
        (project_dir .. "Source/**.ixx"),
    }

    postbuildcommands
    {
        "xcopy \"" .. project_dir .. "Assets\"" .. " \"$(OutDir)Assets\"  /e /s /h /i /y",  -- Game Assets
        "xcopy \"" .. jupiter_dir .. "Assets\\Common\"" .. " \"$(OutDir)Assets\\Common\"  /e /s /h /i /y",    -- Engine Common Assets
    }
