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
# Scripts folder
# -----------------------------------------------------------------------------------------------------
# <ProjectDirectory>/Scripts/GenerateProject.bat
def create_generate_project_files_bat():
	generator_bat = """cd /d "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"

set args="<ProjectName>" "<ProjectDirectory>"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Bin\premake5.exe" <action> %args%

pause
"""
	generator_bat = generator_bat.replace("<ProjectName>", project_name)
	generator_bat = generator_bat.replace("<ProjectDirectory>", project_directory)
	
	# 2022
	with open(project_directory + "/Scripts/GenerateProjectFiles_vs2022.bat", "w") as file:
		vs2022 = generator_bat.replace("<action>", "vs2022")
		file.write(vs2022)

	# Add any other versions here
	

def create_scripts():
	os.makedirs(project_directory + "/Scripts")

	create_generate_project_files_bat()


# -----------------------------------------------------------------------------------------------------
# Source folder
# -----------------------------------------------------------------------------------------------------
def create_application_ixx():
	application_content = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application_Base.h"
#include "Core/Minimal/CoreHeaders.h"

export module <ProjectName>;

#if IS_PLATFORM_WIN64
import jpt.Application_Win64;
#endif

import jpt.Utilities;
import jpt.File.Path;

export class <ProjectName> final : 
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
};

bool <ProjectName>::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	return true;
}

#pragma region Engine-Client Communications
const char* jpt::GetClientDir() { return JPT_CLIENT_DIR; }
const wchar_t* jpt::GetClientDirW() { return JPT_CLIENT_DIR_W; }

jpt::Application_Base* jpt::Application_Base::GetInstance()
{
	static <ProjectName> s_instance;
	return &s_instance;
}
#pragma endregion
"""
	application_content = application_content.replace("<ProjectName>", project_name)		
	with open(project_directory + "/Source/Applications/" + "Application_" + project_name + ".ixx", "w") as file:
	    file.write(application_content)


def create_source():
	os.makedirs(project_directory + "/Source/Applications")

	create_application_ixx()


if __name__ == "__main__":
	get_info()

	create_scripts()
	create_source()

	print("Successfully Created Project: " + project_name + " at " + project_directory)