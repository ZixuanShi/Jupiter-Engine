# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os
import shutil

project_name = ""
project_directory = ""

# -----------------------------------------------------------------------------------------------------
# Get basic information from user
# -----------------------------------------------------------------------------------------------------
def get_info():
	global project_name
	global project_directory
	project_name = input("Enter Project Name: ")
	project_directory = input("Enter Project Directory: ") + "/" + project_name + "/"
	project_directory = project_directory.replace("\\", "/")


# -----------------------------------------------------------------------------------------------------
# Assets folder
# -----------------------------------------------------------------------------------------------------
def create_config():
	os.makedirs(project_directory + "/Assets/Config")

	content_settings = """{
	"window_width": 600,
    "window_height": 400,
    "window_title": "<ProjectName>"
}"""

	content_settings = content_settings.replace("<ProjectName>", project_name)
	with open(project_directory + "/Assets/Config/ProjectSettings.json", "w") as file:
	    file.write(content_settings)

def create_assets():
	os.makedirs(project_directory + "/Assets")

	create_config()

# -----------------------------------------------------------------------------------------------------
# Scripts folder
# -----------------------------------------------------------------------------------------------------
# <ProjectDirectory>/Scripts/GenerateProject.bat
def create_generate_project_files_bat():
	generator_bat = """set args="platform=win64"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Bin\premake5.exe" vs2022 %args%
pause
"""
	
	# 2022
	with open(project_directory + "/Scripts/GenerateProjectFiles_vs2022.bat", "w") as file:
		vs2022 = generator_bat.replace("<action>", "vs2022")
		file.write(vs2022)

	# Add any other versions here

# <ProjectDirectory>/Scripts/premake5.lua
def create_premake_lua():
	premake_lua = """function FindJupiterRootDir()
    local envVar = os.getenv("JUPITER_ENGINE_DIR")
    if envVar then
        print("Jupiter Engine directory found at: " .. envVar)
        return envVar
    else
        print("Jupiter Engine directory not found. Using default path C:/Program Files/Jupiter Technologies/Jupiter-Engine/")
        return "C:/Program Files/Jupiter Technologies/Jupiter-Engine/"
    end
end

include (FindJupiterRootDir() .. "Scripts/premake5.lua")

context.project_name = "<ProjectName>"
context.project_dir  = "<ProjectDirectory>"

GenerateProjectFiles()
"""
	premake_lua = premake_lua.replace("<ProjectName>", project_name)
	premake_lua = premake_lua.replace("<ProjectDirectory>", project_directory)
	with open(project_directory + "/Scripts/premake5.lua", "w") as file:
	    file.write(premake_lua)

def create_scripts():
	os.makedirs(project_directory + "/Scripts")

	create_generate_project_files_bat()
	create_premake_lua()


# -----------------------------------------------------------------------------------------------------
# Source folder
# -----------------------------------------------------------------------------------------------------
def create_application_header():
	content = """#pragma once

#if IS_PLATFORM_WIN64
	#include "Applications/App/Application_Win64.h"
#else
	#include "Applications/App/Application_Base.h"
#endif

class <ProjectName> final : 
#if IS_PLATFORM_WIN64
	public jpt::Application_Win64
#else
	public jpt::Application_Base
#endif
{
private:
	#if IS_PLATFORM_WIN64
		using Super = jpt::Application_Win64;
	#else
		using Super = jpt::Application_Base;
	#endif

public:
	virtual bool PreInit() override;
};"""

	content = content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/Applications/" + project_name + ".h", "w") as file:
	    file.write(content)

def create_application_cpp():
	content = """#include "Applications/<ProjectName>.h"

#include "Core/Minimal/CoreHeaders.h"

import jpt.CoreModules;

bool <ProjectName>::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	return true;
}

#pragma region Engine-Client Communications
constexpr const wchar_t* jpt::File::GetClientDirW() { return JPT_CLIENT_DIR_W; }
constexpr const wchar_t* jpt::File::GetOutputDirW() { return JPT_OUTPUT_DIR_W; }
#pragma endregion"""

	content = content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/Applications/" + project_name + ".cpp", "w") as file:
	    file.write(content)


def create_main_cpp():
	content = """#include "Applications/<ProjectName>.h"

import jpt.EntryPoints;

#if IS_PLATFORM_WIN64

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR launchArgs, int nCmdShow)
{
	<ProjectName> app;
	return jpt::MainImpl_Win64(&app, hInstance, launchArgs, nCmdShow);
}
#else

int main(int argc, char* argv[])
{
	<ProjectName> app;
	return jpt::MainImpl(&app);
}
#endif"""

	content = content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/Main.cpp", "w") as file:
	    file.write(content)


def create_source():
	os.makedirs(project_directory + "/Source/Applications")

	create_application_header()
	create_application_cpp()
	create_main_cpp()


if __name__ == "__main__":
	get_info()

	create_assets()
	create_scripts()
	create_source()

	print("Successfully Created Project: " + project_name + " at " + project_directory)