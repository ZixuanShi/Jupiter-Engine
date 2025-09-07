# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os

g_project_name: str = ""
g_project_directory: str = ""

# -----------------------------------------------------------------------------------------------------
# Get basic information from user
# -----------------------------------------------------------------------------------------------------
def get_info():
	global g_project_name
	global g_project_directory
	g_project_name = input("Enter Project Name: ")
	g_project_directory = input("Enter Project Directory: ") + "/" + g_project_name + "/"
	g_project_directory = g_project_directory.replace("\\", "/")


# -----------------------------------------------------------------------------------------------------
# Assets folder
# -----------------------------------------------------------------------------------------------------
def create_assets():
	os.makedirs(g_project_directory + "/Assets")

# -----------------------------------------------------------------------------------------------------
# Config folder
# -----------------------------------------------------------------------------------------------------
def create_project_settings_json():
	project_settings = """{
}"""

	project_settings = project_settings.replace("<ProjectName>", g_project_name)
	with open(g_project_directory + "/Config/ProjectSettings.json", "w") as file:
	    file.write(project_settings)

def create_config():
	os.makedirs(g_project_directory + "/Config")

	create_project_settings_json()

# -----------------------------------------------------------------------------------------------------
# Scripts folder
# -----------------------------------------------------------------------------------------------------
# <ProjectDirectory>/Scripts/GenerateProjectFiles_platform.py
def create_generate_project_files_script():
	generator_script = """# Copyright Jupiter Technologies, Inc. All Rights Reserved.
import os

if __name__ == "__main__":
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    jupiterRoot = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")
    command = "\\"" + jupiterRoot + "/Tools/Premake/Bin/premake5.exe" + "\\" vs2022"

    os.system(command)
"""
	
	# 2022
	with open(g_project_directory + "/Scripts/generate_project_files_vs2022.py", "w") as file:
		file.write(generator_script)

	# Add any other versions here

# <ProjectDirectory>/Scripts/premake5.lua
def create_premake_lua():
	premake_lua = """-- Copyright Jupiter Technologies, Inc. All Rights Reserved.
function FindJupiterRootDir()
    local envVar = os.getenv("JUPITER_ENGINE_ROOT")
    if envVar then
        print("Jupiter Engine directory found at: " .. envVar)
        return envVar
    else
        print("Jupiter Engine directory not found. Using default path C:/Program Files/Jupiter Technologies/Jupiter-Engine")
        return "C:/Program Files/Jupiter Technologies/Jupiter-Engine"
    end
end

function GetProjectDir()
    local projectDir = path.getdirectory(_SCRIPT)
    projectDir = projectDir:sub(1, projectDir:find("Scripts") - 1)
    return projectDir
end

include (FindJupiterRootDir() .. "/Scripts/premake5.lua")

g_context.project_name = "<ProjectName>"
g_context.project_dir  = GetProjectDir()

GenerateProjectFiles()
"""
	premake_lua = premake_lua.replace("<ProjectName>", g_project_name)
	with open(g_project_directory + "/Scripts/premake5.lua", "w") as file:
	    file.write(premake_lua)

def create_scripts():
	os.makedirs(g_project_directory + "/Scripts")

	create_generate_project_files_script()
	create_premake_lua()


# -----------------------------------------------------------------------------------------------------
# Source folder
# -----------------------------------------------------------------------------------------------------
def create_application_cppm():
	content = """module;

export module Applications_<ProjectName>;

import jpt.Application;

export class Application_<ProjectName> final : public jpt::Application
{
private:
	using Super = jpt::Application;

public:
	virtual bool PreInit() override;
};"""

	content = content.replace("<ProjectName>", g_project_name)
	with open(g_project_directory + "/Source/Applications/Application_" + g_project_name + ".cppm", "w") as file:
	    file.write(content)

def create_application_cpp():
	content = """module;

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

module Applications_<ProjectName>;

import jpt.CoreModules;

bool Application_<ProjectName>::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	return true;
}

JPT_SYNC_CLIENT(<ProjectName>)"""

	content = content.replace("<ProjectName>", g_project_name)
	with open(g_project_directory + "/Source/Applications/Application_" + g_project_name + ".cpp", "w") as file:
	    file.write(content)


def create_source():
	os.makedirs(g_project_directory + "/Source/Applications")

	create_application_cppm()
	create_application_cpp()


if __name__ == "__main__":
	get_info()

	create_assets()
	create_config()
	create_scripts()
	create_source()

	print("Successfully Created Project: " + g_project_name + " at " + g_project_directory)