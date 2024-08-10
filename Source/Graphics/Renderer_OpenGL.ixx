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
import jpt.BouncingValue_1D;

export namespace jpt
{
	class Renderer_OpenGL final : public Renderer_Base
	{
		using Super = Renderer_Base;

	private:
		BouncingValue_1D<float> m_b{ 0.3f, 0.7f, 0.3f, 0.1f };

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
		m_b.Update(deltaSeconds);
	}

	void Renderer_OpenGL::Render()
	{
		Super::Render();

		glClearColor(0.2f, 0.3f, m_b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}