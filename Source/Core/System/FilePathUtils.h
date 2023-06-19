// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

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

	/** @return String for the absolute directory path to Jupiter Engine's root folder */
	JPT_API inline jpt::string  GetEngineDir()  { return JPT_ENGINE_DIR; }

	/** @return Wide string for the absolute directory path to Jupiter Engine's root folder */
	JPT_API inline jpt::wstring GetEngineDirW() { return JPT_ENGINE_DIR_W; }

	/** @return String for the absolute directory path to the client project's output folder */
	JPT_API inline jpt::string  GetOutputDir()  { return FilePathUtils::GetInstance()->GetOutputAssetsPath(); }

	/** @return Wide string for the absolute directory path to the client project's output folder */
	JPT_API inline jpt::wstring GetOutputDirW() { return FilePathUtils::GetInstance()->GetOutputAssetsPathW(); }

	/** @param pDetailPath: Additional path after engine's Assets folder could be any file or asset, like "Common/Shaders/BasicShader.hlsl"
		@return String for the absolute directory path to Jupiter Engine's Assets folder */
	JPT_API inline jpt::string  GetEngineAssetPath(const char* pDetailPath)     { return GetEngineDir()  + "Assets/"  + pDetailPath; }

	/** @param pDetailPath: Additional path after engine's Assets folder could be any file or asset, like "Common/Shaders/BasicShader.hlsl"
		@return Wide string for the absolute directory path to Jupiter Engine's Assets folder */
	JPT_API inline jpt::wstring GetEngineAssetPathW(const wchar_t* pDetailPath) { return GetEngineDirW() + L"Assets/" + pDetailPath; }

	/** @param pDetailPath: Additional path after client project's output Assets folder could be any file or asset, like "Common/Shaders/BasicShader.hlsl"
		@return String for the absolute directory path to client project's output Assets folder */
	JPT_API inline jpt::string  GetOutputAssetPath(const char* pDetailPath)     { return GetOutputDir()  + "Assets/"  + pDetailPath; }

	/** @param pDetailPath: Additional path after client project's output Assets folder could be any file or asset, like "Common/Shaders/BasicShader.hlsl"
		@return Wide string for the absolute directory path to client project's output Assets folder */
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

