// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.File.Base;
import jpt.File.IO;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Optional;

bool UnitTest_FileIO_Read_Engine_English()
{
    jpt::Optional<jpt::File_Base> file = jpt::ReadFile(jpt::ESource::Engine, "Assets/Configs/TestJson.json");
    JPT_ENSURE(file);


    return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(UnitTest_FileIO_Read_Engine_English());

    return true;
}
