// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module jpt.ThreadedRenderer;

import jpt.Renderer_Base;
import jpt.Application_Factories;

import jpt.Thread;
import jpt.ThreadSafeQueue;

import jpt.Function;
import jpt.UniquePtr;
import jpt.Optional;

export namespace jpt
{
	class ThreadedRenderer final : public Thread_Base
	{
	private:
		UniquePtr<Renderer_Base> m_pRenderer;
		ThreadSafeQueue<Function<void()>> m_renderQueue;

	public:
		ThreadedRenderer();
	};

	ThreadedRenderer::ThreadedRenderer()
		: Thread_Base("Threaded Renderer")
		, m_pRenderer(Renderer_Create())
	{
	}
}