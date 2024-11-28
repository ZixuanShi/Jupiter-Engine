// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Window.Create;

import jpt.Event;
import jpt.Window;
import jpt.TypeDefs;

export namespace jpt
{
	class Event_Window_Create : public Event
	{
	private:
		const char* m_title = "New Window";
		int32 m_width = Window::kDefaultWidth;
		int32 m_height = Window::kDefaultHeight;

	public:
		Event_Window_Create() = default;

		Event_Window_Create(const char* title, int32 width, int32 height)
			: m_title(title)
			, m_width(width)
			, m_height(height)
		{
		}

		const char* GetTitle() const { return m_title; }
		int32 GetWidth() const { return m_width; }
		int32 GetHeight() const { return m_height; }
	};
}