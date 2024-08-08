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
		Renderer_Base* renderer = nullptr;
		Graphics::API api = Graphics::API::Unknown;

		auto pickAPI = [&renderer](Graphics::API api) -> Renderer_Base*
			{
				switch (api.Value())
				{
				case Graphics::API::OpenGL:
					return new Renderer_OpenGL();

				default:
					JPT_ERROR("Un-implemented Graphics API: " + api.ToString());
					return nullptr;
				}
			};

		// Check CommandLine for graphics_api
		if (CommandLine::GetInstance().Has("graphics_api"))
		{
			api = CommandLine::GetInstance().Get("graphics_api");
		}
		// Check Assets/Config/ProjectSettings.json
		else if (Optional<JsonMap> settingsOpt = ReadJsonFile(File::FixDependencies("Assets/Config/Settings.json")))
		{
			const JsonMap& settings = settingsOpt.Value();
			if (settings.Has("graphics_api"))
			{
				api = settings["graphics_api"].As<String>();
			}
		}
		// Default based on platform
		else
		{
		#if IS_PLATFORM_WIN64
			api = Graphics::API::OpenGL;
		#else
			JPT_ERROR("No Graphics API specified in CommandLine or ProjectSettings.json.");
			return nullptr;
		#endif
		}

		renderer = pickAPI(api);
		if (renderer == nullptr)
		{
			JPT_ERROR("Failed to create Renderer.");
		}

		return renderer;
	}
}