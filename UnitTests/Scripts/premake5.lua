-- Jupiter workspace
workspace "JupiterUnitTests"
    -- Globals for jupiter workspace
    local jupiter_outputpath = "%{cfg.platform}_%{cfg.buildcfg}"
    local jupiter_basedir = "C:/Program Files/Jupiter Technologies/Jupiter Engine/"   -- Needs to be changed if the Jupiter Engine or Premake5.exe moves.

    -- Config
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
    flags
    {
        "FatalCompileWarnings",
    }

    -- Global filters
    filter "configurations:Debug"
        defines 
        { 
            "IS_DEBUG",
            "JPT_ENABLE_ASSERTS",
            "JPT_ENABLE_MEMORY_TRACKING",       -- Detecting memory leaks, very slow operations
        }
        buildoptions 
        { 
            "/MDd",
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
            "/MD",
        }
        optimize "On"
        symbols "On"

    filter "platforms:Win64"
        defines 
        { 
            "IS_PLATFORM_WIN64" 
        }

    filter "configurations:Release"
        defines 
        { 
            "IS_RELEASE"
        }
        buildoptions 
        {
            "/MD",
        }
        optimize "Full"
        symbols "Off"

    filter "platforms:Win64"
        links
        {
            "d3d12",
            "dxgi",
        }
        defines 
        { 
            "IS_PLATFORM_WIN64" 
        }
        
-- Jupiter Engine
project "Engine"
    kind "SharedLib"

    includedirs
    {
        (jupiter_basedir.. "Source"),
        (jupiter_basedir.. "Source/Core/Building/PreCompiledHeader"),       -- only for JupiterPCH
    }

    files 
    {
        (jupiter_basedir .. "Source/**.h"),
        (jupiter_basedir .. "Source/**.cpp"),
    }

    pchheader ("JupiterPCH.h")
    pchsource (jupiter_basedir .. "Source/Core/Building/PreCompiledHeader/JupiterPCH.cpp")

    defines
    {
        "JPT_BUILD_DLL",
    }

-- Unit Tests
project "UnitTests"
    filter "platforms:Win64"
        kind "WindowedApp"

    includedirs
    {
        (jupiter_basedir .. "Source"),
        (jupiter_basedir.. "Source/Core/Building/PreCompiledHeader"),
        ("../Source")
    }

    links
    {
        "Engine"
    }

    files
    {
        "../Source/**.h",
        "../Source/**.cpp"
    }

    postbuildcommands
    {
        -- Engine dll
        "xcopy \"$(SolutionDir)..\\Generated\\Engine_" .. jupiter_outputpath .. "_Output\\*.dll\"" .. " \"$(OutDir)\"  /d /i /y",

        -- Assets
        "xcopy \"$(SolutionDir)..\\Assets\"" .. " \"$(OutDir)Assets\"  /e /s /h /i /y",  -- Game Assets
        "xcopy \"" .. jupiter_basedir .."Assets\\Common\"" .. " \"$(OutDir)Assets\\Engine\"  /e /s /h /i /y",    -- Engine Assets
    }
