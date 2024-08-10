// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

export module jpt.Renderer_OpenGL;

import jpt.Renderer_Base;
import jpt.Utilities;

export namespace jpt
{
	class Renderer_OpenGL final : public Renderer_Base
	{
		using Super = Renderer_Base;

	private:
		float b = 0.2f;
		bool bIncreasing = true;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Render() override;
	};

	bool Renderer_OpenGL::Init()
	{
		JPT_ENSURE(Super::Init());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			JPT_ERROR("Failed to initialize GLAD");
			return false;
		}

		return true;
	}

	void Renderer_OpenGL::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		if (bIncreasing)
		{
			b += 0.2f * static_cast<float>(deltaSeconds);
		}
		else
		{
			b -= 0.2f * static_cast<float>(deltaSeconds);
		}

		if (b >= 0.8f)
		{
			bIncreasing = false;
		}
		else if (b <= 0.2f)
		{
			bIncreasing = true;
		}
	}

	void Renderer_OpenGL::Render()
	{
		Super::Render();

		glClearColor(0.2f, 0.3f, b, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);
	}
}