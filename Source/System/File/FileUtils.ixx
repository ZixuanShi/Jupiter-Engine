// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Macros.h"
#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.FileUtils;

export namespace jpt
{
	namespace FileUtils
	{
		enum class ESource : uint8
		{
			Unknown,
			Engine,
			Client,
		};

		JPT_ENUM_UINT8(EType,
			Unknown,
			txt,
			json,
		);
	}

	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	const char* GetClientDir();

	/** Replaces directory slashes to platform-correct version */
	template<StringType TString>
	void FixSlashes(TString& path);
}

export namespace jpt
{
	template<StringType TString>
	void FixSlashes(TString& path)
	{
		using TChar = typename TString::TChar;

		path.Replace(JPT_GET_PROPER_STRING(TChar, \\), JPT_GET_PROPER_STRING(TChar, / ));
	}
}