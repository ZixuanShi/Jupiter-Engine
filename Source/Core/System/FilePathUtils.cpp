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

	jpt::string GetFullPathFromDirectory(EAssetDirectoryType directoryType, const char* pPath)
	{
		switch (directoryType)
		{
		case jpt::EAssetDirectoryType::Engine:	return GetEngineDir() + pPath;
		case jpt::EAssetDirectoryType::Project: return GetProjectDir() + pPath;
		case jpt::EAssetDirectoryType::Output:  return GetOutputDir() + pPath;
		default: JPT_ASSERT(false);			return "Unresolved";
		}
	}
	jpt::wstring GetFullPathFromDirectoryW(EAssetDirectoryType directoryType, const wchar_t* pPath)
	{
		switch (directoryType)
		{
		case jpt::EAssetDirectoryType::Engine:	return GetEngineDirW() + pPath;
		case jpt::EAssetDirectoryType::Project: return GetProjectDirW() + pPath;
		case jpt::EAssetDirectoryType::Output:  return GetOutputDirW() + pPath;
		default: JPT_ASSERT(false);			return L"Unresolved";
		}
	}
}