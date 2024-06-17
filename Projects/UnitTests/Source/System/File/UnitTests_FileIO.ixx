// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#pragma execution_character_set("utf-8")

#include "Core/Minimal/Headers.h"

#include <fstream>

export module UnitTests_FileIO;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.StrongPtr;
import jpt.Math;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.Serializer;
import jpt.SerializationUtils;
import jpt.Optional;

import jpt.File;
import jpt.File.Enums;
import jpt.FileIO;
import jpt.File.PathUtils;

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
    jpt::File::MakeDirectory({ ESource::Engine, "Assets/NewDirectory_UnitTest" });
    JPT_ENSURE(Exists({ ESource::Engine, "Assets/NewDirectory_UnitTest" }));

    jpt::File::MakeDirectory({ ESource::Client, "Assets/NewDirectory_UnitTest" });
    JPT_ENSURE(Exists({ ESource::Client, "Assets/NewDirectory_UnitTest" }));

    jpt::File::MakeDirectory({ ESource::Client, L"Assets/新文件夹_UnitTest" });
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
    loader.LoadText({ ESource::Client, "Assets/Configs/TestJson.json" });
    //JPT_LOG(loader.GetText());
    JPT_ENSURE(loader.GetPath().Contains("Assets/Configs/TestJson.json"));

	// Create new one and edit its content
    File saver;
    saver.SetText("Hello, World! I'm a new text file");

    // Save
    saver.SaveText({ ESource::Client, "Assets/NewTextFile_UnitTest.txt" });
    JPT_ENSURE(Exists({ ESource::Client, "Assets/NewTextFile_UnitTest.txt"}));

    // Load again
    loader.LoadText({ ESource::Client, "Assets/NewTextFile_UnitTest.txt" });
    //JPT_LOG(loader.GetText());
	JPT_ENSURE(loader.GetPath().Contains("Assets/NewTextFile_UnitTest.txt"));
    const auto text = loader.GetText();
    JPT_ENSURE(text == "Hello, World! I'm a new text file");

	// Clean up
	jpt::File::Delete({ ESource::Client, "Assets/NewTextFile_UnitTest.txt" });
    JPT_ENSURE(!Exists({ ESource::Client, "Assets/NewTextFile_UnitTest.txt" }));

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

		bool operator==(const Foo& other) const
		{
			return m_bool == other.m_bool &&
				jpt::AreValuesClose(m_double, other.m_double) &&
				jpt::AreStringsSame(m_cstr, other.m_cstr) &&
				jpt::AreStringsSame(m_wcstr, other.m_wcstr);
		}
    };

	Foo foo = { true, 3.14, "ABCDRFG", L"中文" };

    // Save Binary
    File saver;
    saver.SetData(foo);
    saver.SaveBinary<Foo>({ ESource::Client, "Assets/Bin_UnitTest.bin" });

	// Load Binary
    File loader;
	loader.LoadBinary<Foo>({ ESource::Client, "Assets/Bin_UnitTest.bin" });
    const Foo& loaderData = loader.GetData<Foo>();
	JPT_ENSURE(loaderData == foo);

    // Another loader
    File loader2;
	loader2.LoadBinary<Foo>({ ESource::Client, "Assets/Bin_UnitTest.bin" });
	const Foo& loaderData2 = loader2.GetData<Foo>();
    JPT_ENSURE(loaderData2 == foo);

    // Clean
	jpt::File::Delete({ ESource::Client, "Assets/Bin_UnitTest.bin" });

    return true;
}

bool UnitTest_FileIO_Serialization()
{
    struct Foo
    {
		int32 m_int = 0;
		jpt::WString m_string;
		jpt::DynamicArray<int32> m_array;

		void Serialize(jpt::Serializer& serializer) const
        {
            jpt::Serialize(m_int, serializer);
			jpt::Serialize(m_string, serializer);
			//jpt::Serialize(m_array, os);

			// array
			size_t arrayCount = m_array.Count();
            serializer.Write(reinterpret_cast<const char*>(&arrayCount), sizeof(arrayCount));
            serializer.Write(reinterpret_cast<const char*>(m_array.ConstBuffer()), m_array.Size());
        }
        void Deserialize(jpt::Serializer& serializer)
        {
            jpt::Deserialize(m_int, serializer);
            jpt::Deserialize(m_string, serializer);

            // array
			size_t arrayCount = 0;
            serializer.Read(reinterpret_cast<char*>(&arrayCount), sizeof(arrayCount));
			m_array.Resize(arrayCount);
            serializer.Read(reinterpret_cast<char*>(m_array.Buffer()), arrayCount * sizeof(int32));
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

        bool operator==(const Foo& other) const
		{
			return m_int == other.m_int &&
				m_string == other.m_string &&
				m_array == other.m_array;
		}
	};

	// Save
	File saver;
	saver.SetData<Foo>({ 56, L"Hello哥们儿, World! 你弄啥类", { 9,8,6,4,5 } });
	saver.SaveBinary<Foo>({ ESource::Client, "Assets/Serialization_UnitTest.bin" });
	const Foo& saverData = saver.GetData<Foo>();

	// Load
	File loader;
	loader.LoadBinary<Foo>({ ESource::Client, "Assets/Serialization_UnitTest.bin" });
	const Foo& loaderData = loader.GetData<Foo>();
	JPT_ENSURE(saverData == loaderData);

	jpt::File::Delete({ ESource::Client, "Assets/Serialization_UnitTest.bin" });

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
