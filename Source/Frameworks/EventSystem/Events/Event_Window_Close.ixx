// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Window.Close;

import jpt.Event;
import jpt.Window_Base;

export namespace jpt
{
	class Event_Window_Close : public Event
	{
	private:
		Window_Base* m_pWindow;

	public:
		Event_Window_Close(Window_Base* pWindow)
			: m_pWindow(pWindow)
		{
		}

		Window_Base* GetWindow() const { return m_pWindow; }
	};
}