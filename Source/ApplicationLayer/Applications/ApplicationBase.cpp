// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "ApplicationBase.h"

#include "Rendering/Renderers/RendererBase.h"
#include "ApplicationLayer/Windows/Window.h"

namespace jpt
{
	ApplicationBase::ApplicationBase()
		: m_pWindow(WindowBase::Create())
		, m_pRenderer(RendererBase::Create())
		, m_shouldQuit(false)
	{
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

	}

	void ApplicationBase::Update()
	{
	}
}
