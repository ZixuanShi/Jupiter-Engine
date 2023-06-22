// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "FilePathUtils.h"

namespace jpt
{
	FilePathUtils& FilePathUtils::GetInstance()
	{
		static FilePathUtils s_instance;
		return s_instance;
	}

	FilePathUtils::FilePathUtils()
	{
		// String
		char assetsPath[512];
		size_t size = GetModuleFileNameA(nullptr, assetsPath, 512);
		if (size == 0 || size == 512)
		{
			// Method failed or path was truncated.
			throw std::exception();
		}

		char* lastSlash = strrchr(assetsPath, '\\');
		if (lastSlash)
		{
			*(lastSlash + 1) = '\0';
		}

		m_outputAssetsPath.CopyString(assetsPath);

		// Wide String
		wchar_t assetsPathW[512];
		size = GetModuleFileNameW(nullptr, assetsPathW, 512);
		if (size == 0 || size == 512)
		{
			// Method failed or path was truncated.
			throw std::exception();
		}

		wchar_t* lastSlashW = wcsrchr(assetsPathW, L'\\');
		if (lastSlashW)
		{
			*(lastSlashW + 1) = L'\0';
		}

		m_outputAssetsPathW.CopyString(assetsPathW);
	}
}