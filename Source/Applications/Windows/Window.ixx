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
		static constexpr int32 kDefaultWidth = 800;
		static constexpr int32 kDefaultHeight = 600;

	protected:
		uint32 m_fps = 0;

	public:
		virtual ~Window() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init(const char* /*title*/, int32 /*width*/ , int32 /*height*/) { return true; }
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Shutdown() {}

		virtual bool CreateSurface([[maybe_unused]] const DynamicArray<Any>& context) { return true; }

		virtual bool ShouldClose() const { return false; }

		/** @return Vector2, xy are window's screen lengths in pixels */
		virtual Vec2i GetFrameSize() const { JPT_ASSERT(false); return Vec2i(0, 0); }
		
		float GetAspectRatio() const;
		bool IsMinimized() const { return GetFrameSize().x == 0 || GetFrameSize().y == 0; }
	};

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

	float Window::GetAspectRatio() const
	{
		const Vec2i size = GetFrameSize(); 
		return static_cast<float>(size.x) / static_cast<float>(size.y);
	}
}