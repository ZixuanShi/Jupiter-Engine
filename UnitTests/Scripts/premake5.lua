-- Jupiter workspace
workspace "JupiterUnitTests"
    -- Globals for jupiter workspace
    local jupiter_outputpath = "%{cfg.platform}_%{cfg.buildcfg}"
    local jupiter_dir = "C:/Program Files/Jupiter Technologies/Jupiter Engine/"   -- Needs to be changed if the Jupiter Engine or Premake5.exe moves.
    local project_dir = "C:/Program Files/Jupiter Technologies/Jupiter Engine/Unitests/"   -- Needs to be changed if the Game Project moves.-- Config

    configurations 
    { 
        "Debug",        -- Debugging
        "Profiling",    -- Profiling. Same configurations with Release but Profiling code
        "Release",      -- Relese/Shipping
    }
    platforms 
    { 
        "Win64",
    }

    startproject ("UnitTests")

    -- Paths
    location  ("../Visual Studio Files")
    targetdir ("../Generated/%{prj.name}_" .. jupiter_outputpath .. "_Output")
    objdir    ("../Generated/%{prj.name}_" .. jupiter_outputpath .. "_Intermediate")

    -- Programming
    language "C++"
    cppdialect "C++latest"
    warnings "Extra"
    staticruntime "on"
    defines
    {
        ("JPT_ENGINE_DIR=\""..jupiter_dir .. "\""),
        ("JPT_ENGINE_DIR_W=L\""..jupiter_dir .. "\""),
        ("JPT_PROJECT_DIR=\""..project_dir .. "\""),
        ("JPT_PROJECT_DIR_W=L\""..project_dir .. "\""),
        ("NOMINMAX"),   -- To get rid of built-in min/max macros
    }

    -- Global filters
    filter "configurations:not Debug"
        flags
        {
            "FatalCompileWarnings",
        }
        
    filter "configurations:Debug"
        defines 
        { 
            "IS_DEBUG",
            "JPT_ENABLE_ASSERTS",
        }
        buildoptions 
        {
            "/MTd",
        }
        optimize "Debug"
        symbols "On"

    filter "configurations:Profiling"
        defines 
        { 
            "IS_PROFILING"
        }
        buildoptions 
        {
            "/MT",
        }
        optimize "Full"
        symbols "off"

    filter "configurations:Release"
        defines 
        { 
            "IS_RELEASE"
        }
        buildoptions 
        {
            "/MT",
        }
        optimize "Full"
        symbols "off"

    filter "platforms:Win64"
        defines 
        { 
            "IS_PLATFORM_WIN64",
        }

        
-- Jupiter Engine
project "Engine"
    kind "StaticLib"

    includedirs
    {
        (jupiter_dir .. "Source"),
        (jupiter_dir .. "Source/Core/Building/CoreMinimal"),
    }

    files 
    {
        (jupiter_dir .. "Source/**.h"),
        (jupiter_dir .. "Source/**.cpp"),
        (jupiter_dir .. "Source/**.ixx"),
    }

-- Unit Tests
project "UnitTests"
    filter "platforms:Win64"
        kind "WindowedApp"

    includedirs
    {
        (jupiter_dir .. "Source"),
        (jupiter_dir .. "Source/Core/Building/CoreMinimal"),
        ("../Source")
    }

    links
    {
        "Engine.lib",
        "Engine",
        "d3d12",
        "dxgi",
        "d3dcompiler",
    }

    libdirs
    {
        ("$(SolutionDir)../Generated/Engine_" .. jupiter_outputpath .. "_Output/")
    }

    files
    {
        "../Source/**.h",
        "../Source/**.cpp",
        "../Source/**.ixx",
    }

    postbuildcommands
    {
        -- Engine dll
        -- "xcopy \"$(SolutionDir)..\\Generated\\Engine_" .. jupiter_outputpath .. "_Output\\*.dll\"" .. " \"$(OutDir)\"  /d /i /y",

        -- Assets
        "xcopy \"$(SolutionDir)../Assets\"" .. " \"$(OutDir)Assets\"  /e /s /h /i /y",  -- Game Assets
        "xcopy \"" .. jupiter_dir .."Assets/Common\"" .. " \"$(OutDir)Assets\\Common\"  /e /s /h /i /y",    -- Engine Common Assets
    }
