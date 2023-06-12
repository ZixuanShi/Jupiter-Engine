// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "FilePathUtils.h"

namespace jpt
{
	jpt::string GetEngineDir()
	{
		return jpt::string(JPT_ENGINE_DIR);
	}

	jpt::wstring GetEngineDirW()
	{
		return jpt::wstring(JPT_ENGINE_DIR_W);
	}

	jpt::string GetOutputDir()
	{
		return jpt::string(JPT_OUTPUT_DIR);
	}

	jpt::wstring GetOutputDirW()
	{
		return jpt::wstring(JPT_OUTPUT_DIR_W);
	}

	jpt::string GetEngineAssetPath(const char* pDetailPath)
	{
		return GetEngineDir() + "Assets/" + pDetailPath;
	}

	jpt::wstring GetEngineAssetPathW(const wchar_t* pDetailPath)
	{
		return GetEngineDirW() + L"Assets/" + pDetailPath;
	}

	jpt::string GetOutputAssetPath(const char* pDetailPath)
	{
		return GetOutputDir() + "Assets/" + pDetailPath;
	}

	jpt::wstring GetOutputAssetPathW(const wchar_t* pDetailPath)
	{
		return GetOutputDirW() + L"Assets/" + pDetailPath;
	}
}