// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#define ENGINE_DIR_W L##ENGINE_DIR

namespace jpt
{
	class FilePathUtils
	{
	private:
		static FilePathUtils s_instance;

		jpt::string  m_outputAssetsPath;
		jpt::wstring m_outputAssetsPathW;

	public:
		static FilePathUtils* GetInstance() { return &s_instance; }

		const jpt::string&  GetOutputAssetsPath()  const { return m_outputAssetsPath; }
		const jpt::wstring& GetOutputAssetsPathW() const { return m_outputAssetsPathW; }

	private:
		FilePathUtils();
	};

	/* Utils for getting files paths */
	JPT_API inline jpt::string  GetEngineDir()  { return JPT_ENGINE_DIR; }
	JPT_API inline jpt::wstring GetEngineDirW() { return JPT_ENGINE_DIR_W; }
	JPT_API inline jpt::string  GetOutputDir()  { return FilePathUtils::GetInstance()->GetOutputAssetsPath(); }
	JPT_API inline jpt::wstring GetOutputDirW() { return FilePathUtils::GetInstance()->GetOutputAssetsPathW(); }

	/* @return	A string of asset's absolute path */
	JPT_API inline jpt::string  GetEngineAssetPath(const char* pDetailPath)     { return GetEngineDir()  + "Assets/"  + pDetailPath; }
	JPT_API inline jpt::wstring GetEngineAssetPathW(const wchar_t* pDetailPath) { return GetEngineDirW() + L"Assets/" + pDetailPath; }
	JPT_API inline jpt::string  GetOutputAssetPath(const char* pDetailPath)     { return GetOutputDir()  + "Assets/"  + pDetailPath; }
	JPT_API inline jpt::wstring GetOutputAssetPathW(const wchar_t* pDetailPath) { return GetOutputDirW() + L"Assets/" + pDetailPath; }

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

