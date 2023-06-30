// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "ApplicationBase.h"

#include "Rendering/Renderers/RendererBase.h"
#include "ApplicationLayer/Windows/Window.h"

namespace jpt
{
	bool ApplicationBase::PreInit()
	{
		m_pWindow = WindowBase::Create();
		JPT_RETURN_FALSE_IF_LOG(!m_pWindow, "Failed creating window");
		
		m_pRenderer = RendererBase::Create();
		JPT_RETURN_FALSE_IF_LOG(!m_pRenderer, "Failed creating Renderer");

		return true;
	}

	bool ApplicationBase::Init()
	{
		JPT_RETURN_FALSE_IF_LOG(!m_pWindow->Init(), "Failed to Init m_pWindow");
		JPT_RETURN_FALSE_IF_LOG(!m_pRenderer->Init(), "Failed to Init m_pRenderer");

		return true;
	}

	void ApplicationBase::RunGameLoop()
	{
		while (!m_shouldQuit)
		{
			// TODO: Delta Time
			// TODO: Process input
			
			Update();
			Render();
		}
	}

	void ApplicationBase::Clean()
	{
		JPT_CLEAN_THEN_DELETE(m_pRenderer);
		JPT_CLEAN_THEN_DELETE(m_pWindow);
	}

	void ApplicationBase::Render()
	{
		m_pWindow->Render();
	}

	void ApplicationBase::Update()
	{
		m_pWindow->Update();
	}
}
