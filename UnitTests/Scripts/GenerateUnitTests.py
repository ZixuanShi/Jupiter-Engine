# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os
import subprocess

# ----------------------------------------------------------------------------------------------------------------------
# Data
# ----------------------------------------------------------------------------------------------------------------------
subject_name          = ""
categories            = ""
categories_list       = []
script_dir            = ""
source_dir            = ""
unit_test_path        = ""
file_to_add_name      = ""
file_to_add_content   = ""
file_name_template    = "<SubjectName>UnitTests"
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

unit_tests_content_template = """// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module <SubjectName>UnitTests;

/** Unit Test Modules */

export bool RunUnitTests_<SubjectName>()
{
	/** Unit Test Functions */

	return true;
}
"""

# ----------------------------------------------------------------------------------------------------------------------
# Helper Functions
# ----------------------------------------------------------------------------------------------------------------------
def get_data():
    global subject_name       
    global categories         
    global categories_list      
    global script_dir         
    global source_dir         
    global unit_test_path     
    global file_to_add_name   
    global file_to_add_content

    subject_name = input("What's the unit test's subject's name?")
    categories = input("What's the categories of this subject relative to the Source folder? (i.e. Core/Types, Input/Controllers, System/Timing, etc)\n")
    categories_list = categories.split('/')

    file_to_add_name = file_name_template.replace("<SubjectName>", subject_name)
    file_to_add_content = file_content_template.replace("<SubjectName>", subject_name)

    script_dir = os.path.dirname(os.path.realpath(__file__))
    source_dir = script_dir.strip("Scripts") + "Source/"    
    unit_test_path = source_dir + categories + "/" + file_to_add_name + ".ixx"


# Save the new added unit test file with content to Source
def save_unit_test_file():
    if not os.path.isdir(source_dir + categories):
        os.makedirs(source_dir + categories)
        
    with open(unit_test_path, "w") as file:
        file.write(file_to_add_content)


# Run GenerateProjects.bat to adopt the new unit test in Visual Studio
def regenerate_visual_studio_projects():
    os.chdir(script_dir)
    subprocess.run(script_dir + "/GenerateProjects.bat")


# Modify corresponding main_category_UnitTests.ixx to integrate the new unit test module automatically
def update_application_layer():
    main_category  = categories_list[0]
    unit_test_name = "UnitTests_" + main_category + ".ixx"
    unit_test_path = source_dir + "ApplicationLayer/" + unit_test_name

    # Create main_category_UnitTests.ixx under ApplicationLayer if it doesn't exist
    if not os.path.exists(unit_test_path):
        unit_tests_content_content = unit_tests_content_template.replace("<SubjectName>", main_category)
        with open(unit_test_path, "w") as file:
            file.write(unit_tests_content_content)

    # Insert import <SubjectName>UnitTests; under // categories[1], create it if doesn't exist


    # Insert JPT_RETURN_FALSE_IF_ERROR(!Run<SubjectName>UnitTests(), "<SubjectName> Tests Failed");


    # JupiterUnitTestsApplication add UnitTests_Category if there's none

    pass


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
    update_application_layer()
    regenerate_visual_studio_projects()


if __name__ == '__main__':
	main()
