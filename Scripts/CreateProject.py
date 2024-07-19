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
def create_application_communications_ixx():
	communications_content = """// This file overrides the global communication functions through out both engine and client

module;

#include "Application_<ProjectName>.h"

export module ApplicationCommunications;

import jpt.Application_Base;
import jpt.File.Path;

/** Must Overrides Application GetInstance here */
jpt::Application_Base* jpt::Application_Base::GetInstance()
{
	static Application_<ProjectName> s_instance;
	return &s_instance;
}

/** Must Overrides GetClientDir here */
const char* jpt::GetClientDir()
{
	return JPT_CLIENT_DIR;
}
const wchar_t* jpt::GetClientDirW()
{
	return JPT_CLIENT_DIR_W;
}
"""
	communications_content = communications_content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/ApplicationLayer/ApplicationCommunications.ixx", "w") as file:
		file.write(communications_content)


def create_application_header():
	application_content = """#pragma once

#if IS_PLATFORM_WIN64
	import jpt.Application_Win64;
#else
	import jpt.Application_Base;
#endif

class Application_<ProjectName> final : 
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
	virtual bool Init() override final;
};
"""
	application_content = application_content.replace("<ProjectName>", project_name)		
	with open(project_directory + "/Source/ApplicationLayer/" + "Application_" + project_name + ".h", "w") as file:
	    file.write(application_content)


def create_application_cpp():
	application_content = """#include "Application_<ProjectName>.h"

#include "Core/Minimal/CoreHeaders.h"

import jpt.CoreModules;

bool Application_<ProjectName>::Init()
{
	JPT_ENSURE(Super::Init());

	return true;
}
"""
	application_content = application_content.replace("<ProjectName>", project_name)		
	with open(project_directory + "/Source/ApplicationLayer/" + "Application_" + project_name + ".cpp", "w") as file:
	    file.write(application_content)


def create_source():
	os.makedirs(project_directory + "/Source/ApplicationLayer")

	create_application_communications_ixx()
	create_application_header()
	create_application_cpp()


if __name__ == "__main__":
	get_info()

	create_scripts()
	create_source()

	print("Successfully Created Project: " + project_name + " at " + project_directory)