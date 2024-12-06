// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

export module jpt.Renderer_DX12;

import jpt.Renderer;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

import jpt.Window;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

//using namespace jpt::DX12;

export namespace jpt
{
	class Renderer_DX12 final : public Renderer
	{
		using Super = Renderer;

	private:


	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

		virtual void RegisterWindow(Window* pWindow) override;
	};

	bool Renderer_DX12::Init()
	{
		JPT_ENSURE(Super::Init());



		return true;
	}

	void Renderer_DX12::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

	}

	void Renderer_DX12::Shutdown()
	{

		Super::Shutdown();
	}

	void Renderer_DX12::DrawFrame()
	{
		Super::DrawFrame();

	}

	void Renderer_DX12::RegisterWindow(Window* pWindow)
	{
		JPT_INFO("Window registered with DX12 renderer: %lu", pWindow);
	}
}