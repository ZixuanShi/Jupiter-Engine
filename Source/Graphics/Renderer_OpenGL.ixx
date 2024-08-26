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

import jpt.Optional;
import jpt.String;

import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.System.Paths;

export namespace jpt
{
	class Renderer_OpenGL final : public Renderer_Base
	{
		using Super = Renderer_Base;

	private:
		BouncingValue_1D<float> m_b{ 0.3f, 0.7f, 0.3f, 0.1f };
		uint32 m_VAO = 0;
		uint32 m_VBO = 0;
		uint32 m_EBO = 0;
		uint32 m_shaderProgram = 0;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Render() override;
		virtual void Shutdown() override;
	};

	bool Renderer_OpenGL::Init()
	{
		JPT_ENSURE(Super::Init());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			JPT_ERROR("Failed to initialize GLAD");
			return false;
		}

		GLint succeeded = 0;
		char infoLog[512];

		// Vertex Shader
		File::Path vertexShaderPath = File::FixDependencies("Assets/Jupiter_Common/Shaders/vertex.glsl");
		Optional<String> vertexShaderSource = File::ReadTextFile(vertexShaderPath);
		JPT_ASSERT(vertexShaderSource, "Failed reading vertex shader at %ls", vertexShaderPath.ConstBuffer());
		const char* vertexShaderSourceBuffer = vertexShaderSource.Value().ConstBuffer();

		uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSourceBuffer, nullptr);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succeeded);
		if (succeeded == GL_FALSE)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			JPT_ERROR("Vertex shader (%ls) compilation failed: %s", vertexShaderPath.ConstBuffer(), infoLog);
		}

		// Fragment Shader
		File::Path fragmentShaderPath = File::FixDependencies("Assets/Jupiter_Common/Shaders/fragment.glsl");
		Optional<String> fragmentShaderSource = File::ReadTextFile(fragmentShaderPath);
		JPT_ASSERT(fragmentShaderSource, "Failed reading fragment shader at %ls", fragmentShaderPath.ConstBuffer());
		const char* fragmentShaderSourceBuffer = fragmentShaderSource.Value().ConstBuffer();

		uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSourceBuffer, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succeeded);
		if (succeeded == GL_FALSE)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			JPT_ERROR("Fragment shader (%ls) compilation failed: %s", fragmentShaderPath.ConstBuffer(), infoLog);
		}

		// Shader Program
		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);

		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &succeeded);
		if (succeeded == GL_FALSE)
		{
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
			JPT_ERROR("Shader program linking failed: %s", infoLog);
		}

		glUseProgram(m_shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		
		// Triangle Vertices
		float vertices[] = 
		{
			// Left
			0.0f, 0.0f, 0.0f,
			-0.50f, 0.0f, 0.0f,
			-0.25f, 0.5f, 0.0f,

			// Right
			0.50f, 0.5f, 0.0f,
			0.75f, 0.0f, 0.0f,
			0.25f, 0.0f, 0.0f,
		};
		unsigned int indices[] = 
		{
			0, 1, 2,
			3, 4, 5
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		glUseProgram(m_shaderProgram);
		glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // no need to unbind it every time
	}

	void Renderer_OpenGL::Shutdown()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteProgram(m_shaderProgram);

		Super::Shutdown();
	}
}