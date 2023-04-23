-- Jupiter workspace
workspace "JupiterDemoGame"
    -- Globals for jupiter workspace
    local jupiter_outputpath = "%{cfg.platform}_%{cfg.buildcfg}"
    local jupiter_basedir = "../../"   -- Needs to be changed if the Jupiter Engine or Premake5.exe moves

    -- Config
    configurations { "Debug", "Release" }
    platforms { "Win64" }
    startproject "Game"

    -- Paths
    location "../Visual Studio Files"
    targetdir ("../Generated/%{prj.name}_" .. jupiter_outputpath .. "_Output")
    objdir ("../Generated/%{prj.name}_" .. jupiter_outputpath .. "_Intermediate")

    -- Programming
    language "C++"
    cppdialect "C++latest"
    warnings "Extra"

    -- Global filters
    filter "configurations:Debug"
        defines 
        { 
            "IS_DEBUG",
            "JPT_ENABLE_ASSERTS",
            "JPT_ENABLE_MEMORY_TRACKING",       -- Detecting memory leaks, very slow operations
        }
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines 
        { 
            "IS_RELEASE"
        }
        buildoptions "/MD"
        optimize "On"

    filter "platforms:Win64"
        defines { "IS_PLATFORM_WIN64" }


-- Jupiter Engine
project "Engine"
    kind "SharedLib"

    includedirs
    {
        (jupiter_basedir.. "Code"),
        (jupiter_basedir.. "Code/Core/Building"),       -- only for JupiterPCH
    }

    files 
    {
        (jupiter_basedir .. "Code/**.h"),
        (jupiter_basedir .. "Code/**.cpp"),
    }

    pchheader ("JupiterPCH.h")
    pchsource (jupiter_basedir .. "Code/Core/Building/JupiterPCH.cpp")

    defines
    {
        "JPT_BUILD_DLL",
    }


-- Demo Game
project "Game"
    kind "ConsoleApp"

    includedirs
    {
        (jupiter_basedir .. "Code"),
        ("../Code")
    }

    links
    {
        "Engine"
    }

    files
    {
        "../Code/**.h",
        "../Code/**.cpp"
    }

    postbuildcommands
    {
        -- Engine dll
        "xcopy \"$(SolutionDir)..\\Generated\\Engine_" .. jupiter_outputpath .. "_Output\\*.dll\"" .. " \"$(OutDir)\"  /d /i /y",

        -- Assets
        "xcopy \"$(SolutionDir)..\\Assets\"" .. " \"$(OutDir)Assets\"  /e /s /h /i /y"
    }

