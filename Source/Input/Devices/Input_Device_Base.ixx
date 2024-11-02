// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input.Device;

import jpt.Time.TypeDefs;

export namespace jpt::Input
{
	class Device
	{
	public:
		virtual ~Device() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) { }
		virtual void Shutdown() { }
	};
}