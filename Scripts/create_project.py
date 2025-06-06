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

	project_settings = """{
}"""

	project_settings = project_settings.replace("<ProjectName>", project_name)
	with open(project_directory + "/Assets/Config/ProjectSettings.json", "w") as file:
	    file.write(project_settings)

def create_assets():
	os.makedirs(project_directory + "/Assets")

	create_config()

# -----------------------------------------------------------------------------------------------------
# Scripts folder
# -----------------------------------------------------------------------------------------------------
# <ProjectDirectory>/Scripts/GenerateProject.bat
def create_generate_project_files_bat():
	generator_bat = """call "%JUPITER_ENGINE_DIR%Tools\Premake\Bin\premake5.exe" vs2022
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

function GetProjectDir()
    local projectDir = path.getdirectory(_SCRIPT)
    projectDir = projectDir:sub(1, projectDir:find("Scripts") - 1)
    return projectDir
end

include (FindJupiterRootDir() .. "Scripts/premake5.lua")

context.project_name = "<ProjectName>"
context.project_dir  = GetProjectDir()

GenerateProjectFiles()
"""
	premake_lua = premake_lua.replace("<ProjectName>", project_name)
	with open(project_directory + "/Scripts/premake5.lua", "w") as file:
	    file.write(premake_lua)

def create_scripts():
	os.makedirs(project_directory + "/Scripts")

	create_generate_project_files_bat()
	create_premake_lua()


# -----------------------------------------------------------------------------------------------------
# Source folder
# -----------------------------------------------------------------------------------------------------
def create_application_ixx():
	content = """module;

export module Aplications_<ProjectName>;

import jpt.Application;

export class Application_<ProjectName> final : public jpt::Application
{
private:
	using Super = jpt::Application;

public:
	virtual bool PreInit() override;
};"""

	content = content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/Applications/Application_" + project_name + ".ixx", "w") as file:
	    file.write(content)

def create_application_cpp():
	content = """module;

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

module Aplications_<ProjectName>;

import jpt.CoreModules;

bool Application_<ProjectName>::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	return true;
}

JPT_SYNC_CLIENT(<ProjectName>)"""

	content = content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/Applications/Application_" + project_name + ".cpp", "w") as file:
	    file.write(content)


def create_source():
	os.makedirs(project_directory + "/Source/Applications")

	create_application_ixx()
	create_application_cpp()


if __name__ == "__main__":
	get_info()

	create_assets()
	create_scripts()
	create_source()

	print("Successfully Created Project: " + project_name + " at " + project_directory)