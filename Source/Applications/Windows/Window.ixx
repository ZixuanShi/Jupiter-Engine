// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Window;

import jpt.Vector2;
import jpt.Any;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
    class Window
    {
    public:
        static constexpr const char* kDefaultTitle = "Unnamed"; /**< Default window title */
        static constexpr int32 kDefaultWidth  = 800;
        static constexpr int32 kDefaultHeight = 600;

    protected:
        const char* m_title = kDefaultTitle; /**< Window title */
        Vec2i m_frameSize;    /**< xy are window's screen lengths in pixels */

        // FPS
        uint32 m_fps = 0;
        uint32 m_frameCount = 0;
        TimePrecision m_accumulator = 0.0f;

    public:
        virtual ~Window() = default;

        virtual bool Init(const char* title, int32 width, int32 height);
        virtual void Update(TimePrecision deltaSeconds);
        virtual void Shutdown();

    public:
        virtual bool CreateSurface([[maybe_unused]] const DynamicArray<Any>& context);
        virtual bool ShouldClose() const;
        virtual void SetMousePosition(Vec2i);
        virtual void SetCursorVisible(bool);

    public:
        Vec2i GetFrameSize() const;
        void ResizeFrame(Vec2i frameSize);

        float GetAspectRatio() const;
        bool IsMinimized() const;

    private:
        void CalcFPS(TimePrecision deltaSeconds);
    };
}