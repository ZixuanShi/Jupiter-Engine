// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

export module jpt.FileIO;

import jpt.String;
import jpt.Optional;
import jpt.Serializer;

import jpt.File_Enums;
import jpt.FilePath;

import jpt.DynamicArray;

export namespace jpt::File
{
    /** @return        true if path exists in system. Could be either file or directory */
    bool Exists(const Path& absoluteFullPath);

    /** Creates a directory and all necessary parent folders */
    bool MakeDirectory(const Path& absoluteFullPath);

    /** Ensures path's parent folder exists */
    bool EnsureParentDirExists(const Path& absoluteFullPath);

    /** Deletes either file or directory */
    bool Delete(const Path& absoluteFullPath);

    /** @return        String data of a text file */
    Optional<String> ReadTextFile(const Path& path, SerializerMode mode = SerializerMode::Read);

    /** Saves text data to a file */
    bool WriteTextFile(const Path& path, const char* data, size_t sizeInBytes, SerializerMode mode = SerializerMode::WriteAll);
    bool WriteTextFile(const Path& path, const char* data, SerializerMode mode = SerializerMode::WriteAll);
    bool WriteTextFile(const Path& path, const String& data, SerializerMode mode = SerializerMode::WriteAll);

    /** Appends content to file on disk. Write if not present */
    bool AppendTextFile(const Path& path, const char* data, size_t sizeInBytes, SerializerMode mode = SerializerMode::Append);
    bool AppendTextFile(const Path& path, const char* data, SerializerMode mode = SerializerMode::Append);
    bool AppendTextFile(const Path& path, const String& data, SerializerMode mode = SerializerMode::Append);

    /** Loads binary data from a file */
    DynamicArray<char> ReadBinaryFileArray(const Path& path, SerializerMode mode = SerializerMode::ReadBinary);
    
    template<typename T>
    Optional<T> ReadBinaryFile(const Path& path, SerializerMode mode = SerializerMode::ReadBinary)
    {
#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX        
        Serializer serializer(path.ConstBuffer(), mode);
#else
        Serializer serializer(path.GetString<wchar_t>().ConstBuffer(), mode);
#endif

        if (!serializer.IsOpen()) [[unlikely]]
        {
            JPT_ERROR("Failed to open file for reading with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.GetString<wchar_t>().ConstBuffer());
            return Optional<T>();
        }

        T obj;
        serializer.Read(obj);
        return obj;
    }

    /** Saves binary data to a file */
    template<typename T>
    bool WriteBinaryFile(const Path& path, const T& obj, SerializerMode mode = SerializerMode::WriteAll)
    {
#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX        
        Serializer serializer(path.ConstBuffer(), mode);
#else
        Serializer serializer(path.GetString<wchar_t>().ConstBuffer(), mode);
#endif

        if (!serializer.IsOpen()) [[unlikely]]
        {
            JPT_ERROR("Failed to open file for writing with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
            return false;
        }

        serializer.Write(obj);
        return true;
    }
}