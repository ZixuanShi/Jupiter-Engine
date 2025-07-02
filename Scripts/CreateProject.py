# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os

g_projectName: str = ""
g_projectDirectory: str = ""

# -----------------------------------------------------------------------------------------------------
# Get basic information from user
# -----------------------------------------------------------------------------------------------------
def GetInfo():
	global g_projectName
	global g_projectDirectory
	g_projectName = input("Enter Project Name: ")
	g_projectDirectory = input("Enter Project Directory: ") + "/" + g_projectName + "/"
	g_projectDirectory = g_projectDirectory.replace("\\", "/")


# -----------------------------------------------------------------------------------------------------
# Assets folder
# -----------------------------------------------------------------------------------------------------
def CreateAssets():
	os.makedirs(g_projectDirectory + "/Assets")

# -----------------------------------------------------------------------------------------------------
# Config folder
# -----------------------------------------------------------------------------------------------------
def CreateProjectSettingsJson():
	projectSettings = """{
}"""

	projectSettings = projectSettings.replace("<ProjectName>", g_projectName)
	with open(g_projectDirectory + "/Config/ProjectSettings.json", "w") as file:
	    file.write(projectSettings)

def CreateConfig():
	os.makedirs(g_projectDirectory + "/Config")

	CreateProjectSettingsJson()

# -----------------------------------------------------------------------------------------------------
# Scripts folder
# -----------------------------------------------------------------------------------------------------
# <ProjectDirectory>/Scripts/GenerateProjectFiles_platform.py
def CreateGenerateProjectFilesScript():
	generatorScript = """# Copyright Jupiter Technologies, Inc. All Rights Reserved.
import os

if __name__ == "__main__":
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    jupiterRoot = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")
    command = "\\"" + jupiterRoot + "/Tools/Premake/Bin/Premake5.exe" + "\\" vs2022"

    os.system(command)
    os.system("pause")
"""
	
	# 2022
	with open(g_projectDirectory + "/Scripts/GenerateProjectFiles_vs2022.py", "w") as file:
		file.write(generatorScript)

	# Add any other versions here

# <ProjectDirectory>/Scripts/Premake5.lua
def CreatePremakeLua():
	premakeLua = """-- Copyright Jupiter Technologies, Inc. All Rights Reserved.
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

include (FindJupiterRootDir() .. "/Scripts/Premake5.lua")

g_context.project_name = "<ProjectName>"
g_context.project_dir  = GetProjectDir()

GenerateProjectFiles()
"""
	premakeLua = premakeLua.replace("<ProjectName>", g_projectName)
	with open(g_projectDirectory + "/Scripts/Premake5.lua", "w") as file:
	    file.write(premakeLua)

def CreateScripts():
	os.makedirs(g_projectDirectory + "/Scripts")

	CreateGenerateProjectFilesScript()
	CreatePremakeLua()


# -----------------------------------------------------------------------------------------------------
# Source folder
# -----------------------------------------------------------------------------------------------------
def CreateApplicationIxx():
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

	content = content.replace("<ProjectName>", g_projectName)
	with open(g_projectDirectory + "/Source/Applications/Application_" + g_projectName + ".ixx", "w") as file:
	    file.write(content)

def CreateApplicationCpp():
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

	content = content.replace("<ProjectName>", g_projectName)
	with open(g_projectDirectory + "/Source/Applications/Application_" + g_projectName + ".cpp", "w") as file:
	    file.write(content)


def CreateSource():
	os.makedirs(g_projectDirectory + "/Source/Applications")

	CreateApplicationIxx()
	CreateApplicationCpp()


if __name__ == "__main__":
	GetInfo()

	CreateAssets()
	CreateConfig()
	CreateScripts()
	CreateSource()

	print("Successfully Created Project: " + g_projectName + " at " + g_projectDirectory)
	os.system("pause")