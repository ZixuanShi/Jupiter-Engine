# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os
import subprocess

# ----------------------------------------------------------------------------------------------------------------------
# Data
# ----------------------------------------------------------------------------------------------------------------------
subject_name = ""
categories  = ""
main_category = ""
script_dir = ""
source_dir = ""
unit_test_path = ""
file_to_add_name = ""
file_to_add_content = ""
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

# ----------------------------------------------------------------------------------------------------------------------
# Helper Functions
# ----------------------------------------------------------------------------------------------------------------------
def get_data():
    global subject_name
    global file_to_add_name
    global file_to_add_content
    global categories
    global script_dir
    global source_dir
    global unit_test_path
    global main_category

    subject_name = input("What's the unit test's subject's name?")
    file_to_add_name = file_name_template.replace("<SubjectName>", subject_name)
    file_to_add_content = file_content_template.replace("<SubjectName>", subject_name)
    
    # Get categories
    categories = input("What's the categories of this subject relative to the Source folder? (i.e. Core/Types, Input/Controllers, System/Timing, etc)\n")

    # Get unit test file path to add
    script_dir = os.path.dirname(os.path.realpath(__file__))
    source_dir = script_dir.strip("Scripts") + "Source/"
    unit_test_path = source_dir + categories + "/" + file_to_add_name + ".ixx"

    main_category = categories.split()[0]


def save_unit_test_file():
    # Save file to Source
    if not os.path.isdir(source_dir + categories):
        os.makedirs(source_dir + categories)
        
    with open(unit_test_path, "w") as file:
        file.write(file_to_add_content)


def regenerate_visual_studio_projects():
    # Run GenerateProjects.bat to adopt the new unit test in Visual Studio
    os.chdir(script_dir)
    subprocess.run(script_dir + "/GenerateProjects.bat")


# ----------------------------------------------------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------------------------------------------------
def main():
    get_data()

    # Check if it exists already
    if os.path.exists(unit_test_path):
        print(unit_test_path + " has already been created. Quitting")
        return
    else:
        print("Adding <" + file_to_add_name + ">")

    save_unit_test_file()

    # TODO: Modify corresponding main_category_UnitTests.ixx to integrate the new unit test module automatically
    # Create main_category_UnitTests.ixx under ApplicationLayer if it doesn't exist
    # Insert import <SubjectName>UnitTests; under // categories[1], create it if doesn't exist
    # Insert JPT_RETURN_FALSE_IF_ERROR(!Run<SubjectName>UnitTests(), "<SubjectName> Tests Failed");

    regenerate_visual_studio_projects()


if __name__ == '__main__':
	main()
