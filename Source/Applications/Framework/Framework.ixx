// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Framework;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Application_Base;

	class Framework
	{
	public:
		virtual ~Framework() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}
	};
}