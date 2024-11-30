// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Renderer;

import jpt.Window;

import jpt.Time.TypeDefs;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

export namespace jpt
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual bool PreInit();
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) { }
		virtual void Render() { }
		virtual void Shutdown() { }

		virtual void DrawFrame() { }

		virtual void RegisterWindow([[maybe_unused]] Window* pWindow) {}
		virtual void OnWindowResize([[maybe_unused]] const Event_Window_Resize& eventWindowResize) {}
		virtual void OnWindowClose([[maybe_unused]] const Event_Window_Close& eventWindowClose) {}
	};

	bool Renderer::PreInit()
	{
		EventManager::GetInstance().Register<Event_Window_Resize>(this, &Renderer::OnWindowResize);
		EventManager::GetInstance().Register<Event_Window_Close>(this, &Renderer::OnWindowClose);
		return true;
	}
}