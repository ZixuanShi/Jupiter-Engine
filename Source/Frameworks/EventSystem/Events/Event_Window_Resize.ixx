// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Window.Resize;

import jpt.Event;
import jpt.TypeDefs;
import jpt.Window_Base;

export namespace jpt
{
	class Event_Window_Resize : public Event
	{
	private:
		Window_Base* m_pWindow;
		int32 m_width;
		int32 m_height;

	public:
		Event_Window_Resize(Window_Base* pWindow, int32 width, int32 height)
			: m_pWindow(pWindow)
			, m_width(width)
			, m_height(height)
		{
		}

		Window_Base* GetWindow() const { return m_pWindow; }
		int32 GetWidth() const { return m_width; }
		int32 GetHeight() const { return m_height; }
	};
}