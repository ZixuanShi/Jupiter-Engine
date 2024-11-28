// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Renderer;

import jpt.Time.TypeDefs;

import jpt.Event.Manager;
import jpt.Event.Window.Create;
import jpt.Event.Window.Resize;

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

		virtual void OnWindowCreate([[maybe_unused]] const Event_Window_Create& eventWindowCreate) {}
		virtual void OnWindowResize([[maybe_unused]] const Event_Window_Resize& eventWindowResize) {}
	};

	bool Renderer::PreInit()
	{
		EventManager::GetInstance().Register<Event_Window_Create>(this, &Renderer::OnWindowCreate);
		EventManager::GetInstance().Register<Event_Window_Resize>(this, &Renderer::OnWindowResize);
		return true;
	}
}