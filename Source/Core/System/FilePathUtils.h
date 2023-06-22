// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	class FilePathUtils
	{
	private:
		jpt::string  m_outputAssetsPath;
		jpt::wstring m_outputAssetsPathW;

	public:
		static FilePathUtils& GetInstance();

		const jpt::string&  GetOutputAssetsPath()  const { return m_outputAssetsPath; }
		const jpt::wstring& GetOutputAssetsPathW() const { return m_outputAssetsPathW; }

	private:
		FilePathUtils();
	};

	/** Fix slashes in a file path for different platforms */
	template<class StringType>
	inline void FixSlashes(StringType& path)
	{
		if constexpr (jpt::IsSameType<StringType, jpt::string>::Value)
		{
#if IS_PLATFORM_WIN64
			path.replace("\\", "/");
#endif
		}
		else if (jpt::IsSameType<StringType, jpt::wstring>::Value)
		{
#if IS_PLATFORM_WIN64
			path.replace(L"\\", L"/");
#endif
		}
	}

	template<class StringType>
	inline StringType FixedSlashes(StringType&& path)
	{
		if constexpr (jpt::IsSameType<StringType, jpt::string>::Value)
		{
#if IS_PLATFORM_WIN64
			return path.replace("\\", "/");
#endif
		}
		else if (jpt::IsSameType<StringType, jpt::wstring>::Value)
		{
#if IS_PLATFORM_WIN64
			return path.replace(L"\\", L"/");
#endif
		}
	}

	/** Information splited from a path */
	template<typename StringType = jpt::string>
	struct JPT_API PathInfo
	{
		StringType m_folderPath;	/**< Abosulte path to this file's located folder */
		StringType m_name;			/**< Name of the file. Excluding extension */
		StringType m_extension;	/**< Extension representing the type of this file */
	};

	/** Parse a full absolute file path to folder, file name, and extension
		@param pFullAbsolutePath: The absolute full path to this file on the current system	
		@return		Optional path object, exist if succeeded */
	template<typename StringType = jpt::string>
	inline jpt::optional<PathInfo<StringType>> ParseFilePath(const char* pFullAbsolutePath)
	{
		PathInfo<StringType> path;

		jpt::string fullAbsolutePath(pFullAbsolutePath);
		FixSlashes(fullAbsolutePath);

		const size_t lastSlash = fullAbsolutePath.find_last_of("/");
		if (lastSlash == jpt::string::npos)
		{
			JPT_ERROR("Couldn't find the last slash to parse file path: %s", pFullAbsolutePath);
			return jpt::optional<PathInfo<StringType>>();
		}

		path.m_folderPath = fullAbsolutePath.substr(0, lastSlash + 1);

		const size_t lastDot = fullAbsolutePath.find_last_of(".");
		if (lastDot == jpt::string::npos)
		{
			JPT_ERROR("Couldn't find the last dot to parse file extension %s", pFullAbsolutePath);
			return jpt::optional<PathInfo<StringType>>();
		}

		path.m_name = fullAbsolutePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
		path.m_extension = fullAbsolutePath.substr(lastDot + 1);

		return path;
	}

	/** @return String for the absolute directory path to Jupiter Engine's root folder */
	JPT_API inline jpt::string  GetEngineDir()  { return JPT_ENGINE_DIR; }

	/** @return Wide string for the absolute directory path to Jupiter Engine's root folder */
	JPT_API inline jpt::wstring GetEngineDirW() { return JPT_ENGINE_DIR_W; }

	/** @return String for the absolute directory path to the client project's output folder */
	JPT_API inline jpt::string  GetOutputDir()  { return FilePathUtils::GetInstance().GetOutputAssetsPath(); }

	/** @return Wide string for the absolute directory path to the client project's output folder */
	JPT_API inline jpt::wstring GetOutputDirW() { return FilePathUtils::GetInstance().GetOutputAssetsPathW(); }

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
}

