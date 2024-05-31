// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.File_Base;
import jpt.File_Text;
import jpt.FileIO;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_FileIO_Read_Engine_Exists()
{
    using namespace jpt::File;

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


bool UnitTest_FileIO_Read_Engine_English()
{

    return true;
}

export bool RunUnitTests_FileIO()
{
    // Basic tests
    JPT_ENSURE(UnitTest_FileIO_Read_Engine_Exists());

    // Files
    JPT_ENSURE(UnitTest_FileIO_Read_Engine_English());

    return true;
}
