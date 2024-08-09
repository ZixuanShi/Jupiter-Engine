// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input.Device_Base;

import jpt.Time.TypeDefs;

export namespace jpt::Input
{
	class Device_Base
	{
	public:
		virtual ~Device_Base() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) { }
		virtual void Terminate() { }
	};
}