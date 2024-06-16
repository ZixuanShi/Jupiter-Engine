// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#pragma execution_character_set("utf-8")

#include "Core/Minimal/Headers.h"

export module UnitTests_FileIO;

import jpt.File;
import jpt.File.Enums;
import jpt.FileIO;
import jpt.File.PathUtils;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.StrongPtr;
import jpt.Math;
import jpt.HashMap;
import jpt.DynamicArray;

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
    File loader;
    loader.Load({ ESource::Client, "Assets/Configs/TestJson.json" });
    //JPT_LOG(file.GetText());
    JPT_ENSURE(loader.GetPath().Contains("Assets/Configs/TestJson.json"));

	// Create new one and edit its content
    File saver;
    saver.SetText(L"Hello, World! I'm a new text file");

    // Save
    saver.Save({ ESource::Client, "Assets/NewTextFile_UnitTest.txt" });
    JPT_ENSURE(Exists({ ESource::Client, "Assets/NewTextFile_UnitTest.txt"}));

    // Load again
    loader.Load({ ESource::Client, "Assets/NewTextFile_UnitTest.txt" });
    //JPT_LOG(file.GetText());
	JPT_ENSURE(loader.GetPath().Contains("Assets/NewTextFile_UnitTest.txt"));
    JPT_ENSURE(loader.GetText() == "Hello, World! I'm a new text file");

	// Clean up
	//jpt::File::Delete({ ESource::Client, "Assets/NewTextFile.txt" });
    //JPT_ENSURE(!Exists({ ESource::Client, "Assets/NewTextFile.txt" }));

	return true;
}

bool UnitTest_FileIO_BinaryFile()
{
    struct Foo
    {
		bool m_bool = false;
        double m_double = 0.0;
        const char* m_cstr = "";
		const wchar_t* m_wcstr = L"";
    };

    // Save Binary
    File saver;
    saver.SetData<Foo>({ true, 3.14, "ABCDRFG", L"中文" });

    const Foo& saverData = saver.GetData<Foo>();
	JPT_ENSURE(saverData.m_bool == true);
	JPT_ENSURE(jpt::AreValuesClose(saverData.m_double, 3.14));
	JPT_ENSURE(jpt::AreStringsSame(saverData.m_cstr, "ABCDRFG"));
	JPT_ENSURE(jpt::AreStringsSame(saverData.m_wcstr, L"中文"));

    saver.SaveBinary<Foo>({ ESource::Client, L"Assets/中文Bin_UnitTest.bin" });

	// Load Binary
    File loader;
	loader.LoadBinary<Foo>({ ESource::Client, L"Assets/中文Bin_UnitTest.bin" });
    const Foo& loaderData = loader.GetData<Foo>();

    JPT_ENSURE(loaderData.m_bool == true);
    JPT_ENSURE(jpt::AreValuesClose(loaderData.m_double, 3.14));
    JPT_ENSURE(jpt::AreStringsSame(loaderData.m_cstr, "ABCDRFG"));
    JPT_ENSURE(jpt::AreStringsSame(loaderData.m_wcstr, L"中文"));

    // Another loader
    File loader2;
	loader2.LoadBinary<Foo>({ ESource::Client, L"Assets/中文Bin_UnitTest.bin" });
	const Foo& loaderData2 = loader2.GetData<Foo>();

	JPT_ENSURE(loaderData2.m_bool == true);
	JPT_ENSURE(jpt::AreValuesClose(loaderData2.m_double, 3.14));
	JPT_ENSURE(jpt::AreStringsSame(loaderData2.m_cstr, "ABCDRFG"));
	JPT_ENSURE(jpt::AreStringsSame(loaderData2.m_wcstr, L"中文"));

    return true;
}

bool UnitTest_FileIO_Serialization()
{
    struct Foo
    {
		int32 m_int = 0;
		jpt::WString m_string;
		jpt::DynamicArray<int32> m_array;

		void Serialize(std::ofstream& os) const
        {
            // int
			os.write(reinterpret_cast<const char*>(&m_int), sizeof(m_int));

			// string
			size_t stringCount = m_string.Count();
			os.write(reinterpret_cast<const char*>(&stringCount), sizeof(size_t));
            os.write(reinterpret_cast<const char*>(m_string.Buffer()), m_string.Size());

        }
        void Deserialize(std::ifstream& is)
        {
            // int
            is.read(reinterpret_cast<char*>(&m_int), sizeof(m_int));

            // string
			size_t stringCount = 0;
			is.read(reinterpret_cast<char*>(&stringCount), sizeof(size_t));

			wchar_t* buffer = new wchar_t[stringCount + 1];
            is.read(reinterpret_cast<char*>(buffer), stringCount * sizeof(wchar_t));
			buffer[stringCount] = L'\0';

            m_string.MoveString(buffer, stringCount);
        }

        jpt::WString ToWString() const
        {
			jpt::WString result;
			result.Append(L"m_int: ");
			result.Append(jpt::ToString<jpt::WString>(m_int));
			result.Append(L", m_string: ");
			result.Append(m_string);
			result.Append(L", m_array: ");
			result.Append(jpt::ToWString(m_array));
			return result;
        }
	};

	// Save
	File saver;
	saver.SetData<Foo>({ 56, L"Hello哥们儿, World! 你弄啥类", { 9,8,6,4,5 } });
	saver.SaveBinary<Foo>({ ESource::Client, "Assets/Serialization_UnitTest.bin" });
	JPT_LOG(saver.GetData<Foo>());

	// Load
	File loader;
	loader.LoadBinary<Foo>({ ESource::Client, "Assets/Serialization_UnitTest.bin" });
	const Foo& loaderData = loader.GetData<Foo>();
	JPT_LOG(loaderData);

    return true;
}

bool UnitTest_FileIO_DataArray()
{

    return true;
}

bool UnitTest_FileIO_DataMap()
{


    return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(UnitTest_FileIO_Exists());
	JPT_ENSURE(UnitTest_FileIO_Directory());

	JPT_ENSURE(UnitTest_FileIO_TextFile());
	JPT_ENSURE(UnitTest_FileIO_BinaryFile());
	JPT_ENSURE(UnitTest_FileIO_Serialization());

	JPT_ENSURE(UnitTest_FileIO_DataArray());
	JPT_ENSURE(UnitTest_FileIO_DataMap());

    return true;
}
