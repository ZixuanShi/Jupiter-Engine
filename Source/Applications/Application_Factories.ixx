// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module jpt.Application_Factories;

import jpt.Framework_GLFW;
import jpt.Window_GLFW;
import jpt.Renderer_OpenGL;
import jpt.Graphics.Enums;

import jpt.Json;
import jpt.Json.Data;
import jpt.CommandLine;
import jpt.Optional;
import jpt.File.Path;
import jpt.File.Path.Utils;

export namespace jpt
{
	class Framework_Base;
	class Window_Base;
	class Renderer_Base;

	Framework_Base* Framework_Create()
	{
#if IS_PLATFORM_WIN64
		return new Framework_GLFW();
#else
#error "Framework_Create() is not implemented for this platform."
		return nullptr;
#endif
	}

	Window_Base* Window_Create()
	{
#if IS_PLATFORM_WIN64
		return new Window_GLFW();
#else
#error "Window_Create() is not implemented for this platform."
		return nullptr;
#endif
	}

	Renderer_Base* Renderer_Create()
	{
		const File::Path settingsPath = File::FixDependencies("Assets/Config/Settings.json");
		Optional<JsonMap> settingsOpt = ReadJsonFile(settingsPath);

		if (settingsOpt)
		{
			const JsonMap& settings = settingsOpt.Value();

			if (settings.Has("graphics_api"))
			{
				Graphics::API api = settings["graphics_api"].As<String>();
				CommandLine::GetInstance().Set("graphics_api", api.ToString());

				switch (api.Value())
				{
				case Graphics::API::OpenGL:
					return new Renderer_OpenGL();

				case Graphics::API::DirectX12:
					JPT_ERROR("DirectX12 is not implemented.");
					break;

				case Graphics::API::Vulkan:
					JPT_ERROR("Vulkan is not implemented.");
					break;

				default:
					JPT_ERROR("Unknown Graphics API: " + api.ToString());
					break;
				}
			}
		}

		Graphics::API api = Graphics::API::Unknown;

#if IS_PLATFORM_WIN64
		api = Graphics::API::OpenGL;
		CommandLine::GetInstance().Set("graphics_api", api.ToString());

		if (settingsOpt)
		{
			JsonMap& settings = settingsOpt.Value();
			settings.Set("graphics_api", api.ToString());
			WriteJsonFile(settingsPath, settings);
		}

		return new Renderer_OpenGL();
#else
#error "Renderer_Create() is not implemented for this platform."
		return nullptr;
#endif
	}
}