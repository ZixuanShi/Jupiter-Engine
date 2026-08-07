// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Path;

import jpt.PlatformPaths;
import std;

namespace jpt
{
    Path::Path(const char* pRelativePath)
        : Path(std::string_view(pRelativePath))
    {
    }

    Path::Path(std::string_view relativePath)
        : m_absolute(GetRoot() / relativePath)
    {
    }

    std::string Path::GetFileName() const
    {
        return m_absolute.filename().string();
    }
}
