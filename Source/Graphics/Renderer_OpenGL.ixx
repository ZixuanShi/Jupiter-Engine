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

	public:
		virtual bool Init() override;
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

	void Renderer_OpenGL::Render()
	{
		Super::Render();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}