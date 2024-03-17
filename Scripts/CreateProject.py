# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os

project_name = ""
project_directory = ""

# Get Project Name and Directory
def get_info():
	global project_name
	global project_directory
	project_name = input("Enter Project Name: ")
	project_directory = input("Enter Project Directory: ") + "/" + project_name + "/"


# <ProjectDirectory>/Scripts/GenerateProject.bat
def create_generate_project_files_bat():
	generator_bat = """cd /d "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"

set args="<ProjectName>" "<ProjectDirectory>"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Bin\premake5.exe" <action> %args%

pause
"""
	os.makedirs(project_directory + "/Scripts")
	generator_bat = generator_bat.replace("<ProjectName>", project_name)
	generator_bat = generator_bat.replace("<ProjectDirectory>", project_directory)

	# 2022
	with open(project_directory + "/Scripts/GenerateProjectFiles_vs2022.bat", "w") as file:
		vs2022 = generator_bat.replace("<action>", "vs2022")
		file.write(vs2022)

	# Add any other versions here


# <ProjectDirectory>/Source/ApplicationLayer/Main.cpp and ProjectNameApplication.ixx
def create_main_cpp():
	main_content = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Core/Minimal/Headers.h"

import jpt.CoreModules;
import jpt.EntryPoints;

import <ProjectName>Application;

/** Main entry point for different platforms */
#if IS_PLATFORM_WIN64

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lPStr, int nCmdShow)
{
	JPT_LOG("Hello World %s", "<ProjectName>");
	return jpt::MainImplWin64(hInstance, hPrevInstance, lPStr, nCmdShow);
}
#endif
"""
	main_content = main_content.replace("<ProjectName>", project_name)

	os.makedirs(project_directory + "/Source/ApplicationLayer")
	with open(project_directory + "/Source/ApplicationLayer/Main.cpp", "w") as file:
	    file.write(main_content)


def create_application_communications_ixx():
	communications_content = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file overrides the global communication functions through out both engine and client

export module ApplicationCommunications;

import jpt.ApplicationBase;
import jpt.FileUtils;

import <ProjectName>Application;

/** Must Overrides Application GetInstance here */
jpt::ApplicationBase& jpt::ApplicationBase::GetInstance()
{
	static <ProjectName>Application s_instance;
	return s_instance;
}

/** Must Overrides GetClientDir here */
const char* jpt::GetClientDir()
{
	return JPT_CLIENT_DIR;
}
"""
	communications_content = communications_content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/ApplicationLayer/ApplicationCommunications.ixx", "w") as file:
		file.write(communications_content)


def create_application_ixx():
	application_content = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module <ProjectName>Application;

import jpt.ApplicationBase;

export class <ProjectName>Application final : public jpt::ApplicationBase
{
private:
	using Super = jpt::ApplicationBase;

};
"""
	application_content = application_content.replace("<ProjectName>", project_name)		
	with open(project_directory + "/Source/ApplicationLayer/" + project_name + "Application.ixx", "w") as file:
	    file.write(application_content)


if __name__ == "__main__":
	get_info()
	create_generate_project_files_bat()
	create_main_cpp()
	create_application_communications_ixx()
	create_application_ixx()

	print("Successfully Created Project: " + project_name + " at " + project_directory)