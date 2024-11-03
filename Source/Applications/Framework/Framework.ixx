// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Framework;

import jpt.Time.TypeDefs;

export namespace jpt
{
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