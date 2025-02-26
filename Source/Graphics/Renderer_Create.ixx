// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Renderer.Create;

import jpt.Renderer;
import jpt.Graphics.Enums;
import jpt.Renderer_Vulkan;
import jpt.Renderer_DX12;

import jpt.CommandLine;
import jpt.ProjectSettings;

export namespace jpt
{
	Graphics_API FindGraphicsAPI()
	{
		Graphics_API api = Graphics_API::Unknown;

		// Check CommandLine for graphics_api
		if (CommandLine::GetInstance().Has("graphics_api"))
		{
			api = CommandLine::GetInstance().Get<String>("graphics_api");
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
			api = Graphics_API::Vulkan;
#endif
		}

		JPT_ASSERT(api != Graphics_API::Unknown, "No Graphics API specified in CommandLine or ProjectSettings.json.");
		return api;
	}

	Renderer* Renderer_Create(Graphics_API api)
	{
		switch (api.Value())
		{
		case Graphics_API::Vulkan:
			return new Renderer_Vulkan();

		case Graphics_API::DX12:
			return new Renderer_DX12();

		default:
			JPT_ERROR("Un-implemented Graphics API: " + ToString(api));
			return nullptr;
		}
	}
}