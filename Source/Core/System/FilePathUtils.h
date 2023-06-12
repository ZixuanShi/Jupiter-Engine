// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/* Utils for getting files paths */
	JPT_API jpt::string  GetEngineDir();
	JPT_API jpt::wstring GetEngineDirW();
	JPT_API jpt::string  GetOutputDir();
	JPT_API jpt::wstring GetOutputDirW();

	/* @return	A string of asset's absolute path */
	JPT_API jpt::string  GetEngineAssetPath(const char* pDetailPath);
	JPT_API jpt::wstring GetOutputAssetPathW(const wchar_t* pDetailPath);

	template<class StringType>
	inline void FixSlashes(StringType& path)
	{
		if constexpr (jpt::IsSameType<StringType, jpt::string>::Value)
		{
			path.replace("\\", "/");
		}
		else if (jpt::IsSameType<StringType, jpt::wstring>::Value)
		{
			path.replace(L"\\", L"/");
		}
	}

	template<class StringType>
	inline StringType FixedSlashes(StringType&& path)
	{
		if constexpr (jpt::IsSameType<StringType, jpt::string>::Value)
		{
			return path.replace("\\", "/");
		}
		else if (jpt::IsSameType<StringType, jpt::wstring>::Value)
		{
			return path.replace(L"\\", L"/");
		}
	}
}

