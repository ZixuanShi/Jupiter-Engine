# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os
import subprocess

subject_name = ""
category  = ""
file_name_template = "<SubjectName>UnitTests"
file_content_template = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module <SubjectName>UnitTests;

import jpt.CoreModules;

bool UnitTest_<SubjectName>()
{
    jpt::<SubjectName> instance;

    return true;
}

export bool Run<SubjectName>UnitTests()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_<SubjectName>(), "UnitTest_<SubjectName> Failed");

    return true;
}
"""


def main():
    # Get unit test subject name
    subject_name = input("What's the unit test's subject's name?")

    # Replace file template's Subject name with the input name
    file_to_add_name = file_name_template.replace("<SubjectName>", subject_name)
    file_to_add_content = file_content_template.replace("<SubjectName>", subject_name)
    
    print("Adding <" + file_to_add_name + ">")

    # Ask for category
    category = input("What's the category of this subject relative to the Source folder? (i.e. Core/Types, Input/Controllers, etc)\n")

    # Get unit test file path to add
    script_dir = os.path.dirname(os.path.realpath(__file__))
    source_dir = script_dir.strip("Scripts") + "Source/"
    unit_test_path = source_dir + category + "/" + file_to_add_name + ".ixx"
    
    # Check if it exists already
    if os.path.exists(unit_test_path):
        print(unit_test_path + " has already been created. Quitting")
        return

    # Save file to Source
    if not os.path.isdir(source_dir + category):
        os.makedirs(source_dir + category)
        
    with open(unit_test_path, "w") as file:
        file.write(file_to_add_content)

    # Run GenerateProjects.bat to adopt the new unit test in Visual Studio
    os.chdir(script_dir)
    subprocess.run(script_dir + "/GenerateProjects.bat")
    

if __name__ == '__main__':
	main()
