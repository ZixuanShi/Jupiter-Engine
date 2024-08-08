// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input.Device_Base;

export namespace jpt::Input
{
	class Device_Base
	{
	public:
		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update() { }
		virtual void Terminate() { }
	};
}