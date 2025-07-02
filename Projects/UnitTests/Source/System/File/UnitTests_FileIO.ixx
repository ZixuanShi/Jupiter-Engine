// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#pragma execution_character_set("utf-8")

#include "Core/Minimal/CoreHeaders.h"

#include <fstream>

export module UnitTests_FileIO;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.SharedPtr;
import jpt.Math;

import jpt.String;
import jpt.DynamicArray;
import jpt.Pair;
import jpt.HashMap;
import jpt.Serializer;
import jpt.Optional;

import jpt.File_Enums;
import jpt.FileIO;
import jpt.FilePath;
import jpt.FilePathUtils;  
import jpt.SystemPaths;

using namespace jpt::File;

static bool IsSerializeOverridden()
{
    struct Foo {};
    struct Bar
    {
        void Serialize(jpt::Serializer& serializer) const { JPT_IGNORE(serializer); }
        void Deserialize(jpt::Serializer& serializer) { JPT_IGNORE(serializer); }
    };

    static_assert(!jpt::IsSerializeOverridden<int32>);
    static_assert(!jpt::IsSerializeOverridden<char>);
    static_assert(!jpt::IsSerializeOverridden<Foo>);

    static_assert(jpt::IsSerializeOverridden<jpt::String>);
    static_assert(jpt::IsSerializeOverridden<jpt::DynamicArray<int32>>);
    static_assert(jpt::IsSerializeOverridden<jpt::DynamicArray<jpt::String>>);
    static_assert(jpt::IsSerializeOverridden<Bar>);

    return true;
}

bool UnitTests_FileIO_Exists()
{
    // Engine
    jpt::File::WriteTextFile(jpt::File::Combine(Source::Engine, "Config/TextTxt.txt"), "Hello, World!");
    jpt::File::WriteTextFile(jpt::File::Combine(Source::Engine, L"Assets/中文测试/中文文本.txt"), "中文测试");

    JPT_ENSURE(Exists(jpt::File::Combine(Source::Engine, "Config/TextTxt.txt")));
    JPT_ENSURE(!Exists(jpt::File::Combine(Source::Engine, "Config/NotExist.txt")));
    JPT_ENSURE(Exists(jpt::File::Combine(Source::Engine, L"Assets/中文测试/中文文本.txt")));

    jpt::File::Delete(jpt::File::Combine(Source::Engine, "Config/TextTxt.txt"));
    jpt::File::Delete(jpt::File::Combine(Source::Engine, L"Assets/中文测试"));

    // Client
    jpt::File::WriteTextFile(jpt::File::Combine(Source::Client, L"Assets/好家伙/中文文本.txt"), "中文测试");
    JPT_ENSURE(Exists(jpt::File::Combine(Source::Client, L"Assets/好家伙/中文文本.txt")));
    jpt::File::Delete(jpt::System::Paths::GetInstance().GetClientDir() + L"Assets/好家伙");

    return true;
}

bool UnitTests_FileIO_Directory()
{
    // Create
    jpt::File::MakeDirectory(jpt::File::Combine(Source::Engine, "Assets/NewDirectory_UnitTest"));
    JPT_ENSURE(Exists(jpt::File::Combine(Source::Engine, "Assets/NewDirectory_UnitTest")));

    jpt::File::MakeDirectory(jpt::File::Combine(Source::Client, "Assets/NewDirectory_UnitTest"));
    JPT_ENSURE(Exists(jpt::File::Combine(Source::Client, "Assets/NewDirectory_UnitTest")));

    jpt::File::MakeDirectory(jpt::File::Combine(Source::Client, L"Assets/新文件夹_UnitTest"));
    JPT_ENSURE(Exists(jpt::File::Combine(Source::Client, L"Assets/新文件夹_UnitTest")));

    // Destroy
    jpt::File::Delete(jpt::File::Combine(Source::Engine, "Assets/NewDirectory_UnitTest"));
    JPT_ENSURE(!Exists(jpt::File::Combine(Source::Engine, "Assets/NewDirectory_UnitTest")));

    jpt::File::Delete(jpt::File::Combine(Source::Client, "Assets/NewDirectory_UnitTest"));
    JPT_ENSURE(!Exists(jpt::File::Combine(Source::Client, "Assets/NewDirectory_UnitTest")));

    jpt::File::Delete(jpt::File::Combine(Source::Client, L"Assets/新文件夹_UnitTest"));
    JPT_ENSURE(!Exists(jpt::File::Combine(Source::Client, L"Assets/新文件夹_UnitTest")));

    return true;
}

bool UnitTests_FileIO_TextFile()
{
    // Load existing file
    //jpt::String text;
    //jpt::File::ReadTextFile({ ESource::Client, "Config/TestJson.json"), text);
    //JPT_LOG(text);

    const Path path = jpt::File::Combine(Source::Client, "Assets/NewTextFile_UnitTest.txt");

    // Save new file
    jpt::String newContent = "Hello, World! I'm a new text file\n I'm the second line";
    WriteTextFile(path, newContent);
    JPT_ENSURE(Exists(path));

    // Load again
    jpt::Optional<jpt::String> loaded = ReadTextFile(path);
    JPT_ENSURE(loaded.Value() == "Hello, World! I'm a new text file\n I'm the second line");

    // Clean up
    Delete(path);
    JPT_ENSURE(!Exists(path));

    return true;
}

static bool FileIO_AppendTextFile()
{
    const Path path = jpt::File::Combine(Source::Client, "Assets/StackFile_UnitTest.txt");
    jpt::String newContent;

    // Save new file
    newContent = "1";
    jpt::File::AppendTextFile(path, newContent);
    JPT_ENSURE(Exists(path));

    newContent = "2";
    jpt::File::AppendTextFile(path, newContent);

    newContent = "3\n";
    jpt::File::AppendTextFile(path, newContent);

    newContent = "4";
    jpt::File::AppendTextFile(path, newContent);

    // Load
    jpt::String loaded = jpt::File::ReadTextFile(path).Value();
    JPT_ENSURE(loaded == "123\n4");

    // Clean up
    Delete(path);

    return true;
}

bool UnitTests_FileIO_BinaryFile()
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

    const Path path = jpt::File::Combine(Source::Client, "Assets/Bin_UnitTest.bin");
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

bool UnitTests_FileIO_Serialization()
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

    const Path path = jpt::File::Combine(Source::Client, "Assets/Serialization_UnitTest.bin");

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

static bool Array_String_Text()
{
    jpt::DynamicArray<jpt::String> data = { "Hello", "World", "I'm", "a", "DynamicArray" };

    const Path path = jpt::File::Combine(Source::Client, "Assets/ArrayText_UnitTest.txt");

    // Save
    WriteTextFile(path, jpt::ToString(data));

    // Load
    jpt::Optional<jpt::String> loaded = ReadTextFile(path);
    JPT_ENSURE(jpt::ToString(data) == loaded.Value());

    // Clean
    Delete(path);
    JPT_ENSURE(!Exists(path));

    return true;
}

static bool Array_String_Serialization()
{
    jpt::DynamicArray<jpt::String> data = { "Hello", "World", "I'm", "a", "DynamicArray" };

    const Path path = jpt::File::Combine(Source::Client, "Assets/ArraySerialization_UnitTest.bin");

    // Save
    WriteBinaryFile(path, data);

    // Load
    jpt::Optional<jpt::DynamicArray<jpt::String>> loaded = ReadBinaryFile<jpt::DynamicArray<jpt::String>>(path);
    JPT_ENSURE(data == loaded.Value());

    // Clean
    Delete(path);
    JPT_ENSURE(!Exists(path));

    return true;
}

static bool Array_Float_Text()
{
    jpt::DynamicArray<float64> data = { 3.14, 2.718, 1.618, 0.618, 0.577 };

    const Path path = jpt::File::Combine(Source::Client, "Assets/ArrayText_UnitTest.txt");

    // Save
    WriteTextFile(path, jpt::ToString(data));

    // Load
    jpt::Optional<jpt::String> loaded = ReadTextFile(path);
    JPT_ENSURE(jpt::ToString(data) == loaded.Value());

    // Clean
    Delete(path);
    JPT_ENSURE(!Exists(path));

    return true;
}

static bool Array_Float_Serialization()
{
    jpt::DynamicArray<float64> data = { 3.14, 2.718, 1.618, 0.618, 0.577 };

    const Path path = jpt::File::Combine(Source::Client, "Assets/ArraySerialization_UnitTest.bin");

    // Save
    WriteBinaryFile(path, data);

    // Load
    jpt::Optional<jpt::DynamicArray<float64>> loaded = ReadBinaryFile<jpt::DynamicArray<float64>>(path);
    JPT_ENSURE(data == loaded.Value());

    // Clean
    Delete(path);
    JPT_ENSURE(!Exists(path));

    return true;
}

static bool FileIO_HashMap_Text()
{
    const Path path = jpt::File::Combine(Source::Client, "Assets/HashMapText_UnitTest.txt");
    jpt::HashMap<jpt::String, int32> data;

    data["Hello"] = 1;
    data["World"] = 2;
    data["I'm"] = 3;
    data["a"] = 4;
    data["HashMap"] = 5;

    // Save
    WriteTextFile(path, jpt::ToString(data));

    // Load
    jpt::Optional<jpt::String> loaded = ReadTextFile(path);
    JPT_ENSURE(jpt::ToString(data) == loaded.Value());

    // Clean
    Delete(path);
    JPT_ENSURE(!Exists(path));

    return true;
}

static bool FileIO_HashMap_Serialization()
{
    const Path path = jpt::System::Paths::GetInstance().GetOutputDir() + "HashMapSerialization_UnitTest.bin";
    jpt::HashMap<jpt::String, int32> data;

    data["Hello"] = 1;
    data["World"] = 2;
    data["I'm"] = 3;
    data["a"] = 4;
    data["HashMap"] = 5;

    // Save
    WriteBinaryFile(path, data);

    // Load
    jpt::Optional<jpt::HashMap<jpt::String, int32>> loaded = ReadBinaryFile<jpt::HashMap<jpt::String, int32>>(path);
    JPT_ENSURE(data == loaded.Value());

    // Clean
    Delete(path);
    JPT_ENSURE(!Exists(path));

    return true;
}

export bool RunUnitTests_FileIO()
{
    JPT_ENSURE(IsSerializeOverridden());

    JPT_ENSURE(UnitTests_FileIO_Exists());
    JPT_ENSURE(UnitTests_FileIO_Directory());

    JPT_ENSURE(UnitTests_FileIO_TextFile());
    JPT_ENSURE(FileIO_AppendTextFile());
    JPT_ENSURE(UnitTests_FileIO_BinaryFile());
    JPT_ENSURE(UnitTests_FileIO_Serialization());

    JPT_ENSURE(Array_String_Text());
    JPT_ENSURE(Array_String_Serialization());
    JPT_ENSURE(Array_Float_Text());
    JPT_ENSURE(Array_Float_Serialization());

    JPT_ENSURE(FileIO_HashMap_Text());
    JPT_ENSURE(FileIO_HashMap_Serialization());

    return true;
}
