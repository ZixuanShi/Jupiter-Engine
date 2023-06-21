// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "File.h"

namespace jpt
{
    File::File(const char* pFullAbsolutePath, size_t sizeInMB /*= kInvalidValue<size_t>*/)
        : m_path(pFullAbsolutePath)
        , m_sizeInMB(sizeInMB)
        , m_lastSlashIndex(m_path.find_last_of("/"))
        , m_dotIndex(m_path.find_last_of("."))
    {
    }

    jpt::string_view File::GetFolderPath() const
    {
        return jpt::string_view(m_path.c_str(), m_lastSlashIndex);
    }

    jpt::string_view File::GetName() const
    {
        return jpt::string_view(m_path.c_str() + m_lastSlashIndex + 1, m_path.size() - m_dotIndex);
    }

    jpt::string_view File::GetExtension() const
    {
        return jpt::string_view(m_path.c_str() + m_dotIndex + 1);
    }
}