// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Window.Close;

import jpt.Event;
import jpt.Window;

export namespace jpt
{
	class Event_Window_Close : public Event
	{
	private:
		Window* m_pWindow;

	public:
		Event_Window_Close(Window* pWindow)
			: m_pWindow(pWindow)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
	};
}