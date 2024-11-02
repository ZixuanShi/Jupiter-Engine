// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Window;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Application;

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}
	};
}