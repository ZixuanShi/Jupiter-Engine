// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Window_Base;

import jpt.Time.TypeDefs;
import jpt.TypeDefs;

export namespace jpt
{
	class Application_Base;

	class Window_Base
	{
	public:
		virtual ~Window_Base() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}
	};
}