// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Renderer_Base;

export namespace jpt
{
	class Renderer_Base
	{
	public:
		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Render() { }
		virtual void Terminate() { }
	};
}