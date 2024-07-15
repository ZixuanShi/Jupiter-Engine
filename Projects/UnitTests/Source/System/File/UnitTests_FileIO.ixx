// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#pragma execution_character_set("utf-8")

#include "Core/Minimal/CoreHeaders.h"

#include <fstream>

export module UnitTests_FileIO;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.StrongPtr;
import jpt.Math;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.Serializer;
import jpt.Optional;

import jpt.File.Enums;
import jpt.FileIO;

using namespace jpt::File;

bool UnitTest_FileIO_Exists()
{
    // Engine
    JPT_ENSURE(Exists({ ESource::Engine, "Assets/Configs/TextTxt.txt" }));
    JPT_ENSURE(!Exists({ ESource::Engine, "Assets/Configs/NotExist.txt" }));
    JPT_ENSURE(Exists({ ESource::Engine, L"Assets/中文测试/中文文本.txt" }));

    // Client
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
    //jpt::String text;
    //jpt::File::ReadTextFile({ ESource::Client, "Assets/Configs/TestJson.json" }, text);
    //JPT_LOG(text);

    const Path path = { ESource::Client, "Assets/NewTextFile_UnitTest.txt" };

    // Save new file
    jpt::String newContent = "Hello, World! I'm a new text file";
	WriteTextFile(path, newContent);
    JPT_ENSURE(Exists(path));

    // Load again
    jpt::Optional<jpt::String> loaded = ReadTextFile(path);
    JPT_ENSURE(loaded.Value() == "Hello, World! I'm a new text file");

	// Clean up
	Delete(path);
    JPT_ENSURE(!Exists(path));

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

	const Path path = { ESource::Client, "Assets/Bin_UnitTest.bin" };
	Foo saver = { true, 3.14, "ABCDRFG", L"中文" };

    // Save Binary
    WriteBinaryFile(path, saver);

	// Load Binary
    jpt::Optional<Foo> loader = ReadBinaryFile<Foo>(path);    
	JPT_ENSURE(loader.Value() == saver);

    // Another loader
    jpt::Optional<Foo> loader2 = ReadBinaryFile<Foo>(path);
    JPT_ENSURE(loader2.Value() == saver);

    // Clean
	Delete(path);
    JPT_ENSURE(!Exists(path));

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
			serializer.Write(m_int);
			serializer.Write(m_string);
			serializer.Write(m_array);
        }
        void Deserialize(jpt::Serializer& serializer)
        {
			serializer.Read(m_int);
			serializer.Read(m_string);
			serializer.Read(m_array);
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

	const Path path = { ESource::Client, "Assets/Serialization_UnitTest.bin" };

	// Save
    Foo saver = { 56, L"Hello哥们儿, World! 你弄啥类", { 9,8,6,4,5 } };
	WriteBinaryFile(path, saver);

	// Load
    jpt::Optional<Foo> loader = ReadBinaryFile<Foo>(path);
	JPT_ENSURE(saver == loader.Value());

	// Clean
	Delete(path);
    JPT_ENSURE(!Exists(path));

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
