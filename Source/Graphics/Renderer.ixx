// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Renderer;

import jpt.Time.TypeDefs;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;

export namespace jpt
{
	class Renderer
	{
	protected:
		bool m_framebufferResized = false;

	public:
		virtual ~Renderer() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init();
		virtual void Update(TimePrecision) { }
		virtual void Render() { }
		virtual void Shutdown() { }

		virtual void DrawFrame() { }

		virtual void OnWindowResized(const Event_Window_Resize& eventWindowResize);
	};

	bool Renderer::Init()
	{
		EventManager::GetInstance().Register<Event_Window_Resize>(this, &Renderer::OnWindowResized);
		return true;
	}

	void Renderer::OnWindowResized([[maybe_unused]] const Event_Window_Resize& eventWindowResize)
	{
		m_framebufferResized = true;
	}
}