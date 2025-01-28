// Copyright Jupiter Technologies]= Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

export module jpt.RawInput_Win32;

import jpt.RawInput;
import jpt.Input.Enums;

import jpt.Constants;
import jpt.HashMap;
import jpt.ToString;
import jpt.TypeDefs;

export namespace jpt::Input
{
	class RawInput_Win32 : public RawInput
	{
		using Super = RawInput;

	private:
		HashMap<Key, uint32> m_toWin32Keys;
		HashMap<uint32, Key> m_fromWin32Keys;

		HashMap<MouseButton, uint32> m_toWin32MouseButtons;
		HashMap<uint32, MouseButton> m_fromWin32MouseButtons;

	public:
		virtual bool PreInit() override;
	};

	bool RawInput_Win32::PreInit()
	{
		JPT_ENSURE(Super::PreInit());

		return true;
	}
}