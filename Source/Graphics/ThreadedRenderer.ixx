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

		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};

	ThreadedRenderer::ThreadedRenderer()
		: Thread_Base("ThreadedRenderer")
		, m_pRenderer(Renderer_Create())
	{
	}

	void ThreadedRenderer::Init()
	{
		JPT_LOG("Initializing ThreadedRenderer");
		m_pRenderer->Init();
	}

	void ThreadedRenderer::Update()
	{
		if (Optional<Function<void()>> task = m_renderQueue.TryDequeue())
		{
			task.Value()();
		}
	}

	void ThreadedRenderer::Shutdown()
	{
		JPT_LOG("Shutting down ThreadedRenderer");
		m_pRenderer->Shutdown();
	}
}