// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Json;

import jpt.String;
import jpt.Optional;

import jpt.JsonData;
import jpt.FilePath;

export namespace jpt
{
    constexpr String ParseKeyStr(const String& line);
    constexpr String ParseValueStr(const String& line);
    constexpr JsonData ParseValueData(const String& valueStr);

    /** Reads a json file from disk. Initialize all the data to memory and assign to root json object then return it */
    Optional<JsonMap> ReadJsonFile(const File::Path& path);

    void WriteJsonFile(const File::Path& path, const JsonMap& jsonRoot);
}