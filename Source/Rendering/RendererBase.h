// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/* Base class for renderer.Implemented by sub - class in specific graphics API */
	class JPT_API RendererBase
	{
	public:
		/* @return A pointer to a created window based on current platform */
		static RendererBase* Create();

		virtual bool Init() { return true; }
		virtual void Update() {}
		virtual void Render() {}
		virtual void Clean() {}
	};
}

