// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.ProjectSettings;

import jpt.Json;
import jpt.Json.Data;
import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.Optional;
import jpt.CommandLine;

export namespace jpt
{
    /** Get & Set config at File::FixDependencies("Assets/Config/ProjectSettings.json")
        Source of truth. Runtime should use this instead of command line */
    class ProjectSettings
    {
    private:
        JsonMap m_settings;

    public:
        JPT_DECLARE_SINGLETON(ProjectSettings);

        bool Load();
        void Save();

        template<typename T>
        bool TryGet(const String& key, T& value) const;

        template<typename T>
        const T& Get(const String& key) const;

        template<typename T>
        const T& Get(const String& key, const T& defaultValue) const;

        void Set(const String& key, const JsonData& value = JsonData());

        void Erase(const String& key);
    };

    bool ProjectSettings::Load()
    {
        const File::Path projectSettingsJson = File::FixDependencies("Assets/Config/ProjectSettings.json");
        Optional<JsonMap> settings = ReadJsonFile(projectSettingsJson);
        if (!settings)
        {
            return false;
        }

        m_settings = Move(settings.Value());

        // Override settings with command line
        for (const auto& [key, value] : CommandLine::GetInstance().GetArgs())
        {
            if (m_settings.Has(key))
            {
                JPT_LOG("Overriding ProjectSettings key: %s with value: %s", key.ConstBuffer(), ToString(value).ConstBuffer());

                m_settings.Add(key, value);
            }
        }

        return true;
    }

    void ProjectSettings::Save()
    {
        const File::Path projectSettingsJson = File::FixDependencies("Assets/Config/ProjectSettings.json");
        WriteJsonFile(projectSettingsJson, m_settings);
    }

    void ProjectSettings::Set(const String& key, const JsonData& value /* = JsonData()*/)
    {
        m_settings[key] = value;
    }

    void ProjectSettings::Erase(const String& key)
    {
        JPT_ASSERT(m_settings.Has(key), "ProjectSettings doesn't exist \"%s\"", key.ConstBuffer());
        m_settings.Erase(key);
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
        JPT_ASSERT(m_settings.Has(key));
        return m_settings[key].As<T>();
    }

    template<typename T>
    const T& ProjectSettings::Get(const String& key, const T& defaultValue) const
    {
        if (m_settings.Has(key))
        {
            return m_settings[key].As<T>();
        }

        return defaultValue;
    }
}