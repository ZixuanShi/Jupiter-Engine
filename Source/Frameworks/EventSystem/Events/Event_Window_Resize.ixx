// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event.Window.Resize;

import jpt.Event;
import jpt.TypeDefs;
import jpt.Window;

export namespace jpt
{
    class Event_Window_Resize : public Event
    {
    private:
        Window* m_pWindow;
        int32 m_width;
        int32 m_height;

    public:
        Event_Window_Resize(Window* pWindow, int32 width, int32 height)
            : m_pWindow(pWindow)
            , m_width(width)
            , m_height(height)
        {
        }

        Window* GetWindow() const { return m_pWindow; }
        int32 GetWidth() const { return m_width; }
        int32 GetHeight() const { return m_height; }

        bool IsMinimized() const { return m_width == 0 || m_height == 0; }
    };
}