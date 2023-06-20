// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "File.h"

namespace jpt
{
    File::File(const char* pFullAbsolutePath, size_t sizeInMB /*= kInvalidValue<size_t>*/)
        : m_sizeInMB(sizeInMB)
    {
        JPT_UNUSED(pFullAbsolutePath);
    }

    File::File(const char* pFolderPath, const char* pName, const char* pExtension, size_t sizeInMB /*= kInvalidValue<size_t>*/)
        : m_folderPath(pFolderPath)
        , m_name(pName)
        , m_extension(pExtension)
        , m_sizeInMB(sizeInMB)
    {
    }
}