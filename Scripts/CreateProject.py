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
def create_generator_bat():
	generate_project_bat_content = """cd "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Scripts"
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Generated\premake5.exe" vs2022 "<ProjectName>" "<ProjectDirectory>"
pause
"""
	os.makedirs(project_directory + "/Scripts")
	generate_project_bat_content = generate_project_bat_content.replace("<ProjectName>", project_name)
	generate_project_bat_content = generate_project_bat_content.replace("<ProjectDirectory>", project_directory)
	with open(project_directory + "/Scripts/GenerateProjects.bat", "w") as file:
		file.write(generate_project_bat_content)


# <ProjectDirectory>/Source/ApplicationLayer/Main.cpp and ProjectNameApplication.ixx
def create_main_cpp():
	main_content = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Core/Minimal/Headers.h"

import jpt.CoreModules;
import jpt.EntryPoints;
import <ProjectName>Application;

/** Must Overrides Application GetInstance here */
jpt::ApplicationBase& jpt::ApplicationBase::GetInstance()
{
	static <ProjectName>Application s_instance;
	return s_instance;
}

/** Main entry point for different platforms */
#if IS_PLATFORM_WIN64

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lPStr, int nCmdShow)
{
	return jpt::MainImplWin64(hInstance, hPrevInstance, lPStr, nCmdShow);
}
#endif
"""
	main_content = main_content.replace("<ProjectName>", project_name)

	os.makedirs(project_directory + "/Source/ApplicationLayer")
	with open(project_directory + "/Source/ApplicationLayer/Main.cpp", "w") as file:
	    file.write(main_content)


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
	create_generator_bat()
	create_main_cpp()
	create_application_ixx()