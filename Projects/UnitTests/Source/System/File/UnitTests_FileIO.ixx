// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#pragma execution_character_set("utf-8")

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.File_Base;
import jpt.File_Text;
import jpt.FileIO;
import jpt.File.PathUtils;
import jpt.TypeDefs;
import jpt.Utilities;

using namespace jpt::File;

bool UnitTest_FileIO_Exists()
{
    // Engine
    JPT_ENSURE(Exists(ESource::Engine, "Assets/Configs/TestJson.json"));
    JPT_ENSURE(Exists(ESource::Engine, "Assets/Configs/TextTxt.txt"));
    JPT_ENSURE(!Exists(ESource::Engine, "Assets/Configs/NotExist.txt"));
    JPT_ENSURE(Exists(ESource::Engine, L"Assets/中文测试/中文文本.txt"));

    // Client
    JPT_ENSURE(Exists(ESource::Client, "Assets/Configs/TestJson.json"));
    JPT_ENSURE(Exists(ESource::Client, L"Assets/好家伙/中文文本.txt"));
    
    return true;
}

bool UnitTest_FileIO_Directory()
{
    // Create
	jpt::File::CreateDirectory(ESource::Engine, "Assets/NewDirectory");
	JPT_ENSURE(Exists(ESource::Engine, "Assets/NewDirectory"));

	jpt::File::CreateDirectory(ESource::Client, "Assets/NewDirectory");
	JPT_ENSURE(Exists(ESource::Client, "Assets/NewDirectory"));

    // Destroy
	jpt::File::DeleteDirectory(ESource::Engine, "Assets/NewDirectory");
	JPT_ENSURE(!Exists(ESource::Engine, "Assets/NewDirectory"));

	jpt::File::DeleteDirectory(ESource::Client, "Assets/NewDirectory");
	JPT_ENSURE(!Exists(ESource::Client, "Assets/NewDirectory"));

    return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(UnitTest_FileIO_Exists());

	JPT_ENSURE(UnitTest_FileIO_Directory());

    return true;
}
