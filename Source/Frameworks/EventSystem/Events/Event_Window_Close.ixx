// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event_Window_Close;

import jpt.Event;

export namespace jpt
{
    class Window;

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