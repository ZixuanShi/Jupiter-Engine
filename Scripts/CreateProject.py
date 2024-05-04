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
	generator_bat = """
call "C:\Program Files\Jupiter Technologies\Jupiter-Engine\Tools\Premake\Bin\premake5.exe" <action>
pause
"""
	# 2022
	with open(project_directory + "/Scripts/GenerateProjectFiles_vs2022.bat", "w") as file:
		vs2022 = generator_bat.replace("<action>", "vs2022")
		file.write(vs2022)

	# Add any other versions here
 

def copy_premake5_lua():
	destination = project_directory + "/Scripts/premake5.lua"
	shutil.copyfile("C:/Program Files/Jupiter Technologies/Jupiter-Engine/Scripts/premake5.lua", destination)
	
	content = ""
	with open(destination, "r") as file:
		content = file.read()

	content = content.replace("<ProjectName>", project_name)
	content = content.replace("<ProjectDirectory>", project_directory)

	with open(destination, "w") as file:
		file.write(content)

def create_scripts():
	os.makedirs(project_directory + "/Scripts")

	create_generate_project_files_bat()
	copy_premake5_lua()


# -----------------------------------------------------------------------------------------------------
# Source folder
# -----------------------------------------------------------------------------------------------------
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
	JPT_LOG("Hello World %s. Powered by Jupiter Engine", "<ProjectName>");
	return jpt::MainImplWin64(hInstance, hPrevInstance, lPStr, nCmdShow);
}
#endif
"""
	main_content = main_content.replace("<ProjectName>", project_name)
	with open(project_directory + "/Source/ApplicationLayer/Main.cpp", "w") as file:
	    file.write(main_content)


def create_application_communications_ixx():
	communications_content = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file overrides the global communication functions through out both engine and client

export module ApplicationCommunications;

import jpt.Application.Base;
import jpt.File.Path.Utils;

import <ProjectName>Application;

/** Must Overrides Application GetInstance here */
jpt::Application_Base& jpt::Application_Base::GetInstance()
{
	static <ProjectName>Application s_instance;
	return s_instance;
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


def create_application_ixx():
	application_content = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module <ProjectName>Application;

import jpt.Application.Base;

export class <ProjectName>Application final : public jpt::Application_Base
{
private:
	using Super = jpt::Application_Base;

};
"""
	application_content = application_content.replace("<ProjectName>", project_name)		
	with open(project_directory + "/Source/ApplicationLayer/" + project_name + "Application.ixx", "w") as file:
	    file.write(application_content)


def create_source():
	os.makedirs(project_directory + "/Source/ApplicationLayer")

	create_main_cpp()
	create_application_communications_ixx()
	create_application_ixx()


if __name__ == "__main__":
	get_info()

	create_scripts()
	create_source()

	print("Successfully Created Project: " + project_name + " at " + project_directory)