// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"

export module jpt.File.Path.Helpers;

import jpt.TypeTraits;

export namespace jpt::File
{
	/** Designed to be implemented in client project's application
		@return Client's project root directory. */
	constexpr const wchar_t* GetClientDirW();
	constexpr const wchar_t* GetOutputDirW();

	/** Replaces directory slashes to platform-correct version */
	template<typename TString>
	constexpr void FixSeparators(TString& path)
	{
		using TChar = typename TString::TChar;

#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
		path.Replace(JPT_GET_PROPER_STRING(TChar, / ), JPT_GET_PROPER_STRING(TChar, \\));
#else
		path.Replace(JPT_GET_PROPER_STRING(TChar, \\), JPT_GET_PROPER_STRING(TChar, / ));
#endif
	}

	template<typename TString>
	constexpr TString GetSeparator()
	{
		using TChar = typename TString::TChar;

#if IS_PLATFORM_WIN64 || IS_PLATFORM_XBOX
		return JPT_GET_PROPER_STRING(TChar, \\);
#else
		return JPT_GET_PROPER_STRING(TChar, / );
#endif
	}
}