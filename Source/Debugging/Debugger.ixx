// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module jpt.Debugger;

import jpt.AssertImpl;

export namespace jpt
{
	class Debugger
	{
		JPT_DECLARE_SINGLETON(Debugger);

	private:
		AssertImpl m_assertImpl;

	public:
		bool PreInit();

	public:
		bool IsDebuggerPresent() const;
	};
}