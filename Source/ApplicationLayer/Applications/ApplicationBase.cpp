#include "JupiterPCH.h"
#include "ApplicationBase.h"

#include "Rendering/RendererBase.h"
#include "ApplicationLayer/Windows/Window.h"

namespace jpt
{
	ApplicationBase::ApplicationBase()
		: m_pWindow(WindowBase::Create())
		, m_pRenderer(RendererBase::Create())
	{
	}

	bool ApplicationBase::Init()
	{
		if (!PreInit())
		{
			return false;
		}

		if (!m_pWindow->Init())
		{
			return false;
		}

		if (!m_pRenderer->Init())
		{
			return false;
		}

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
		m_pRenderer->Clean();
		JPT_SAFE_DELETE(m_pRenderer);

		m_pWindow->Clean();
		JPT_SAFE_DELETE(m_pWindow);
	}

	void ApplicationBase::Render()
	{

	}

	void ApplicationBase::Update()
	{
	}
}
