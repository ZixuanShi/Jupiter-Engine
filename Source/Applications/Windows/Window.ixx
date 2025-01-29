// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

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
		static constexpr int32 kDefaultWidth  = 800;
		static constexpr int32 kDefaultHeight = 600;

	protected:
		Vec2i m_frameSize;	/**< xy are window's screen lengths in pixels */
		uint32 m_fps = 0;

	public:
		virtual ~Window() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init(const char* title, int32 width, int32 height);
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Shutdown() {}

		virtual bool CreateSurface([[maybe_unused]] const DynamicArray<Any>& context) { JPT_ASSERT(false); return true; }
		virtual bool ShouldClose() const { JPT_ASSERT(false); return false; }

		virtual void SetMousePosition(Vec2i) { JPT_ASSERT(false); }

	public:
		Vec2i GetFrameSize() const;
		void ResizeFrame(Vec2i frameSize);
		
		float GetAspectRatio() const;
		bool IsMinimized() const;
	};

	bool Window::Init(const char*, int32 width, int32 height)
	{
		m_frameSize = Vec2i(width, height);
		return true;
	}

	void Window::Update(TimePrecision deltaSeconds)
	{
		static uint32 frameCount = 0;
		static TimePrecision accumulator = 0.0;

		++frameCount;
		accumulator += deltaSeconds;

		if (accumulator >= 1.0)
		{
			m_fps = frameCount;
			frameCount = 0;
			accumulator = 0.0;
		}
	}

	Vec2i Window::GetFrameSize() const
	{
		return m_frameSize;
	}

	void Window::ResizeFrame(Vec2i frameSize)
	{
		m_frameSize = frameSize;
	}

	float Window::GetAspectRatio() const
	{
		return static_cast<float>(m_frameSize.x) / static_cast<float>(m_frameSize.y);
	}

	bool Window::IsMinimized() const
	{
		return m_frameSize.x == 0 || m_frameSize.y == 0;
	}
}