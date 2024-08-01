// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Framework_Base;

import jpt.Time.TypeDefs;

namespace jpt
{
	export class Framework_Base
	{
	public:
		virtual ~Framework_Base() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Terminate() {}
	};
}