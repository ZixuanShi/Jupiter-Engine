// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module jpt.Framework.Create;

import jpt.Framework;
import jpt.Framework.Enums;
import jpt.Framework_GLFW;

import jpt.CommandLine;
import jpt.ProjectSettings;

export namespace jpt
{
	Framework* Framework_Create()
	{
		Framework* framework = nullptr;
		Framework_API api = Framework_API::Unknown;

		auto pickAPI = [&framework](Framework_API api) -> Framework*
			{
				switch (api.Value())
				{
				case Framework_API::GLFW:
					return new Framework_GLFW();

				default:
					JPT_ERROR("Un-implemented Framework API: " + api.ToString());
					return nullptr;
				}
			};

		// Check CommandLine for framework_api
		if (CommandLine::GetInstance().Has("framework_api"))
		{
			api = CommandLine::GetInstance().Get("framework_api");
		}
		// Check Assets/Config/ProjectSettings.json project settings
		else if (String frameworkApi; ProjectSettings::GetInstance().TryGet("framework_api", frameworkApi))
		{
			api = frameworkApi;
		}
		// Default based on platform
		else
		{
#if IS_PLATFORM_WIN64
			api = Framework_API::GLFW;
#else
			JPT_ERROR("No Framework API specified in CommandLine or ProjectSettings.json.");
			return nullptr;
#endif
		}

		framework = pickAPI(api);
		if (framework == nullptr)
		{
			JPT_ERROR("Failed to create Framework.");
		}

		ProjectSettings::GetInstance().Set("framework_api", api.ToString());
		return framework;
	}
}