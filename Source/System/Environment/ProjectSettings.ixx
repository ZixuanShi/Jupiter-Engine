// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module jpt.ProjectSettings;

import jpt.Json;
import jpt.Json.Data;
import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.Optional;

export namespace jpt
{
	/** Get & Set config at File::FixDependencies("Assets/Config/ProjectSettings.json")
		Source of truth. Runtime should use this instead of command line */
	class ProjectSettings
	{
	private:
		JsonMap m_settings;

	public:
		SINGLETON_DECLARATION(ProjectSettings);

		bool PreInit();
		void Terminate();

		template<typename T>
		bool TryGet(const String& key, T& value) const;

		template<typename T>
		const T& Get(const String& key) const;

		void Set(const String& key, const JsonData& value);
	};

	bool ProjectSettings::PreInit()
	{
		const File::Path projectSettingsJson = File::FixDependencies("Assets/Config/ProjectSettings.json");
		if (Optional<JsonMap> settings = ReadJsonFile(projectSettingsJson))
		{
			m_settings = settings.Value();
			return true;
		}

		return false;
	}

	void ProjectSettings::Terminate()
	{
		const File::Path projectSettingsJson = File::FixDependencies("Assets/Config/ProjectSettings.json");
		WriteJsonFile(projectSettingsJson, m_settings);
	}

	void ProjectSettings::Set(const String& key, const JsonData& value)
	{
		m_settings.Set(key, value);
	}

	template<typename T>
	bool ProjectSettings::TryGet(const String& key, T& value) const
	{
		if (m_settings.Has(key))
		{
			value = m_settings[key].As<T>();
			return true;
		}

		return false;
	}

	template<typename T>
	const T& ProjectSettings::Get(const String& key) const
	{
		return m_settings[key].As<T>();
	}
}