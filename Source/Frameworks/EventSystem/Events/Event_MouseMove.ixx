// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.MouseMove;

import jpt.Event;
import jpt.TypeDefs;
import jpt.Window;

export namespace jpt
{
	class Event_Mouse_Move : public Event
	{
	private:
		Window* m_pWindow = nullptr;	/**< Window that the event occurred */
		double m_x = 0;
		double m_y = 0;

	public:
		Event_Mouse_Move(Window* pWindow, double x, double y)
			: m_pWindow(pWindow)
			, m_x(x)
			, m_y(y)
		{
		}

		Window* GetWindow() const { return m_pWindow; }
		double GetX() const { return m_x; }
		double GetY() const { return m_y; }
	};
}