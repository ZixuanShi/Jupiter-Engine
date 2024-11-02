// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module jpt.Renderer.Create;

import jpt.Renderer;
import jpt.Graphics.Enums;

import jpt.CommandLine;
import jpt.ProjectSettings;

export namespace jpt
{
	Renderer* Renderer_Create()
	{
		Renderer* renderer = nullptr;
		Graphics::API api = Graphics::API::Unknown;

		auto pickAPI = [&renderer](Graphics::API api) -> Renderer*
			{
				switch (api.Value())
				{
				case Graphics::API::Vulkan:


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
		// Check Assets/Config/ProjectSettings.json project settings
		else if (String graphicsApi; ProjectSettings::GetInstance().TryGet("graphics_api", graphicsApi))
		{
			api = graphicsApi;
		}
		// Default based on platform
		else
		{
#if IS_PLATFORM_WIN64
			api = Graphics::API::Vulkan;
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

		ProjectSettings::GetInstance().Set("graphics_api", api.ToString());
		return renderer;
	}
}