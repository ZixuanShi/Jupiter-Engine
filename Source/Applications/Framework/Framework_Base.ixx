// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Framework_Base;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Application_Base;

	class Framework_Base
	{
	public:
		virtual ~Framework_Base() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}
	};
}