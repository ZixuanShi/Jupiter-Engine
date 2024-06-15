// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#pragma execution_character_set("utf-8")

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.File_Base;
import jpt.File_Text;
import jpt.File.Enums;
import jpt.FileIO;
import jpt.File.PathUtils;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.StrongPtr;

using namespace jpt::File;

bool UnitTest_FileIO_Exists()
{
    // Engine
    JPT_ENSURE(Exists({ ESource::Engine, "Assets/Configs/TestJson.json" }));
    JPT_ENSURE(Exists({ ESource::Engine, "Assets/Configs/TextTxt.txt" }));
    JPT_ENSURE(!Exists({ ESource::Engine, "Assets/Configs/NotExist.txt" }));
    JPT_ENSURE(Exists({ ESource::Engine, L"Assets/中文测试/中文文本.txt" }));

    // Client
    JPT_ENSURE(Exists({ ESource::Client, "Assets/Configs/TestJson.json" }));
    JPT_ENSURE(Exists({ ESource::Client, L"Assets/好家伙/中文文本.txt" }));
    
    return true;
}

bool UnitTest_FileIO_Directory()
{
    // Create
    jpt::File::CreateDirectory({ ESource::Engine, "Assets/NewDirectory_UnitTest" });
    JPT_ENSURE(Exists({ ESource::Engine, "Assets/NewDirectory_UnitTest" }));

    jpt::File::CreateDirectory({ ESource::Client, "Assets/NewDirectory_UnitTest" });
    JPT_ENSURE(Exists({ ESource::Client, "Assets/NewDirectory_UnitTest" }));

    jpt::File::CreateDirectory({ ESource::Client, L"Assets/新文件夹_UnitTest" });
    JPT_ENSURE(Exists({ ESource::Client, L"Assets/新文件夹_UnitTest" }));

    // Destroy
    jpt::File::Delete({ ESource::Engine, "Assets/NewDirectory_UnitTest" });
    JPT_ENSURE(!Exists({ ESource::Engine, "Assets/NewDirectory_UnitTest" }));

    jpt::File::Delete({ ESource::Client, "Assets/NewDirectory_UnitTest" });
    JPT_ENSURE(!Exists({ ESource::Client, "Assets/NewDirectory_UnitTest" }));

    jpt::File::Delete({ ESource::Client, L"Assets/新文件夹_UnitTest" });
    JPT_ENSURE(!Exists({ ESource::Client, L"Assets/新文件夹_UnitTest" }));

    return true;
}

bool UnitTest_FileIO_TextFile()
{
	// Load existing file
    jpt::StrongPtr<File_Text> pFile = File_Text::Load({ ESource::Client, "Assets/Configs/TestJson.json" });
    //JPT_LOG(pFile->GetText());
    JPT_ENSURE(pFile->GetPath().Contains("Assets/Configs/TestJson.json"));

    // Create new one
    jpt::StrongPtr<File_Text> pNewFile = new File_Text;

    // Edit content
	pNewFile->GetText() = L"Hello, World! I'm a new text file";

    // Save
	pNewFile->Save({ ESource::Client, "Assets/NewTextFile.txt" });
    JPT_ENSURE(Exists({ ESource::Client, "Assets/NewTextFile.txt"}));

    // Load again
	pFile = File_Text::Load({ ESource::Client, "Assets/NewTextFile.txt" });
    //JPT_LOG(pFile->GetText());
	JPT_ENSURE(pFile->GetPath().Contains("Assets/NewTextFile.txt"));
    JPT_ENSURE(pFile->GetText() == "Hello, World! I'm a new text file");

	// Clean up
	jpt::File::Delete({ ESource::Client, "Assets/NewTextFile.txt" });
    JPT_ENSURE(!Exists({ ESource::Client, "Assets/NewTextFile.txt" }));

	return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(UnitTest_FileIO_Exists());
	JPT_ENSURE(UnitTest_FileIO_Directory());

	JPT_ENSURE(UnitTest_FileIO_TextFile());

    return true;
}
