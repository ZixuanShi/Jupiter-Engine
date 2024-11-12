// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Renderer;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) { }
		virtual void Render() { }
		virtual void Shutdown() { }

		virtual void DrawFrame() { }
	};
}