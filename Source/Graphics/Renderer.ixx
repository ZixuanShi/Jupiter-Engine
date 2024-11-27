// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Renderer;

import jpt.Time.TypeDefs;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;

export namespace jpt
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual bool PreInit();
		virtual bool Init();
		virtual void Update(TimePrecision) { }
		virtual void Render() { }
		virtual void Shutdown() { }

		virtual void DrawFrame() { }

		virtual void OnWindowResized([[maybe_unused]] const Event_Window_Resize& eventWindowResize) {}
	};

	bool Renderer::PreInit()
	{
		return true;
	}

	bool Renderer::Init()
	{
		EventManager::GetInstance().Register<Event_Window_Resize>(this, &Renderer::OnWindowResized);
		return true;
	}
}