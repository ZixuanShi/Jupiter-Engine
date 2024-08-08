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
	/** Get & Set config at File::FixDependencies("Assets/Config/Settings.json")
		framework_api
		graphics_api */
	class ProjectSettings
	{
	private:
		JsonMap m_settings;
		bool m_hasSettings = false;

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
		const File::Path projectSettingsJson = File::FixDependencies("Assets/Config/Settings.json");
		if (Optional<JsonMap> settings = ReadJsonFile(projectSettingsJson))
		{
			m_settings = settings.Value();
			m_hasSettings = true;
			return true;
		}

		return false;
	}

	void ProjectSettings::Terminate()
	{
		if (m_hasSettings)
		{
			const File::Path projectSettingsJson = File::FixDependencies("Assets/Config/Settings.json");
			WriteJsonFile(projectSettingsJson, m_settings);
		}	
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