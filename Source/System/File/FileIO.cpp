// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Utilities.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

#include <filesystem>
#include <fstream>

module jpt.FileIO;

import jpt.StringHelpers;

namespace jpt::File
{
    bool Exists(const Path& absoluteFullPath)
    {
        if (absoluteFullPath.IsEmpty()) [[unlikely]]
        {
            return false;
        }

        std::error_code errorCode;
        const bool result = std::filesystem::exists(absoluteFullPath.ConstBuffer(), errorCode);
        if (errorCode) [[unlikely]]
        {
            JPT_ERROR("Error checking if file exists (%ls): %s", absoluteFullPath.ConstBuffer(), errorCode.message().c_str());
        }

        return result;
    }

    bool MakeDirectory(const Path& absoluteFullPath)
    {
        std::error_code errorCode;
        std::filesystem::create_directories(absoluteFullPath.ConstBuffer(), errorCode);
        if (errorCode) [[unlikely]]
        {
            JPT_ERROR("Error creating directory (%ls): %s", absoluteFullPath.ConstBuffer(), errorCode.message().c_str());
            return false;
        }

        return true;
    }

    bool EnsureParentDirExists(const Path& absoluteFullPath)
    {
        if (absoluteFullPath.IsEmpty()) [[unlikely]]
        {
            return false;
        }

        const Path parentPath = absoluteFullPath.GetParent();
        if (!Exists(parentPath)) [[unlikely]]
        {
            return MakeDirectory(parentPath);
        }

        return true;
    }

    bool Delete(const Path& absoluteFullPath)
    {
        std::error_code errorCode;
        const bool result = std::filesystem::remove_all(absoluteFullPath.ConstBuffer(), errorCode);
        if (errorCode) [[unlikely]]
        {
            JPT_ERROR("Error deleting file (%ls): %s", absoluteFullPath.ConstBuffer(), errorCode.message().c_str());
        }

        return result;
    }

    Optional<String> ReadTextFile(const Path& path, SerializerMode mode /*= SerializerMode::Read*/)
    {
#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX        
        Serializer serializer(path.ConstBuffer(), mode);
#else
        Serializer serializer(path.GetString<wchar_t>().ConstBuffer(), mode);
#endif

        if (!serializer.IsOpen()) [[unlikely]]
        {
            JPT_ERROR("Failed to open file for reading with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.GetString<wchar_t>().ConstBuffer());
            return Optional<String>();
        }

        char* buffer = serializer.ReadText();

        String content;
        content.MoveString(buffer);
        return content;
    }

    bool WriteTextFile(const Path& path, const char* data, size_t sizeInBytes, SerializerMode mode /*= SerializerMode::WriteAll*/)
    {
        EnsureParentDirExists(path);
#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX        
        Serializer serializer(path.ConstBuffer(), mode);
#else
        Serializer serializer(path.GetString<wchar_t>().ConstBuffer(), mode);
#endif

        if (!serializer.IsOpen()) [[unlikely]]
        {
            JPT_ERROR("Failed to open file for writing with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.GetString<wchar_t>().ConstBuffer());
            return false;
        }

        serializer.Write(data, sizeInBytes);
        return true;
    }
    bool WriteTextFile(const Path& path, const char* data, SerializerMode mode /*= SerializerMode::WriteAll*/)
    {
        return WriteTextFile(path, data, FindCharsCount(data) * sizeof(char), mode);
    }
    bool WriteTextFile(const Path& path, const String& data, SerializerMode mode /*= SerializerMode::WriteAll*/)
    {
        return WriteTextFile(path, data.ConstBuffer(), data.Size(), mode);
    }

    bool AppendTextFile(const Path& path, const char* data, size_t sizeInBytes, SerializerMode mode /*= SerializerMode::Append*/)
    {
        if (!EnsureParentDirExists(path)) [[unlikely]]
        {
            return false;
        }

#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX        
        Serializer serializer(path.ConstBuffer(), mode);
#else
        Serializer serializer(path.GetString<wchar_t>().ConstBuffer(), mode);
#endif

        if (!serializer.IsOpen()) [[unlikely]]
        {
            JPT_ERROR("Failed to open file for writing with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.GetString<wchar_t>().ConstBuffer());
            return false;
        }

        serializer.Write(data, sizeInBytes);
        return true;
    }
    bool AppendTextFile(const Path& path, const char* data, SerializerMode mode /*= SerializerMode::Append*/)
    {
        return AppendTextFile(path, data, FindCharsCount(data) * sizeof(char), mode);
    }
    bool AppendTextFile(const Path& path, const String& data, SerializerMode mode /*= SerializerMode::Append*/)
    {
        return AppendTextFile(path, data.ConstBuffer(), data.Size(), mode);
    }

    DynamicArray<char> ReadBinaryFileArray(const Path& path, SerializerMode mode /*= SerializerMode::ReadBinary*/)
    {
#if IS_PLATFORM_WINDOWS || IS_PLATFORM_XBOX        
        Serializer serializer(path.ConstBuffer(), mode);
#else
        Serializer serializer(path.GetString<wchar_t>().ConstBuffer(), mode);
#endif

        if (!serializer.IsOpen()) [[unlikely]]
        {
            JPT_ERROR("Failed to open file for reading with SerializerMode::%u: %ls", static_cast<uint32>(mode), path.ConstBuffer());
            return {};
        }

        const size_t fileSize = serializer.GetSize();
        DynamicArray<char> buffer(fileSize);

        serializer.Read(buffer.Buffer(), fileSize);

        serializer.Close();
        return buffer;
    }
}