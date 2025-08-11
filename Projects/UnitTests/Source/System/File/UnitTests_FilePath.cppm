// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_FilePath;

import jpt.FilePath;
import jpt.TypeDefs;
import jpt.Utilities;

bool GetParent()
{
    jpt::File::Path path;

    path = "C:/Jupiter-Engine/Source";
    JPT_ENSURE(path.GetParent() == "C:/Jupiter-Engine/");

    path = "C:/Jupiter-Engine/Source/";
    JPT_ENSURE(path.GetParent() == "C:/Jupiter-Engine/");

    path = "C:/Jupiter-Engine/Source/main.cpp";
    JPT_ENSURE(path.GetParent() == "C:/Jupiter-Engine/Source/");

    return true;
}

bool GetName()
{
    jpt::File::Path path;

    path = "C:/Jupiter-Engine/Source";
    JPT_ENSURE(path.GetName() == "Source");

    path = "C:/Jupiter-Engine/Source/";
    JPT_ENSURE(path.GetName() == "Source");

    path = "C:/Jupiter-Engine/Source/Main.cpp";
    JPT_ENSURE(path.GetName() == "Main.cpp");

    return true;
}

export bool RunUnitTests_FilePath()
{
    JPT_ENSURE(GetParent());
    JPT_ENSURE(GetName());

    return true;
}
