// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Win64.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

import jpt.CommandLine;
import jpt.String.Helpers;

namespace jpt
{
	bool Application_Win64::PreInit()
	{
		JPT_ENSURE(Super::PreInit());
		JPT_ENSURE(m_hInstance != nullptr);

		wchar_t exePath[MAX_PATH];
		GetModuleFileName(m_hInstance, exePath, MAX_PATH);

		String exePathStr;
		exePathStr.MoveString(ToChars(exePath));
		CommandLine::GetInstance().Add("exe_path", exePathStr);

		return true;
	}
}