// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module jpt.ThreadedRenderer;

import jpt.Renderer;
import jpt.Renderer.Create;

import jpt.Thread;
import jpt.ThreadSafeQueue;

import jpt.Function;
import jpt.UniquePtr;
import jpt.Optional;

export namespace jpt
{
	class ThreadedRenderer final : public Thread
	{
	private:
		UniquePtr<Renderer> m_pRenderer;
		ThreadSafeQueue<Function<void()>> m_renderQueue;

	public:
		ThreadedRenderer();
	};

	ThreadedRenderer::ThreadedRenderer()
		: Thread("Threaded Renderer")
		, m_pRenderer(Renderer_Create())
	{
	}
}