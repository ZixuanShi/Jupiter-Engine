// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

module jpt.ProjectSettings;

import jpt.Optional;

import jpt.Json;
import jpt.LaunchArgs;
import jpt.FilePath;
import jpt.FilePathUtils;

namespace jpt
{
    bool ProjectSettings::Load()
    {
        const File::Path projectSettingsJson = File::FixDependencies("Config/ProjectSettings.json");
        Optional<JsonMap> settings = ReadJsonFile(projectSettingsJson);
        if (!settings)
        {
            return false;
        }

        m_settings = Move(settings.Value());

        JPT_INFO("Loaded ProjectSettings from: \"%s\"", ToString(projectSettingsJson).ConstBuffer());
        JPT_INFO(m_settings);

        // Override settings with command line
        for (const auto& [key, value] : LaunchArgs::GetInstance().GetArgs())
        {
            if (m_settings.Has(key))
            {
                JPT_INFO("Overriding ProjectSettings key: \"%s\" with value: %s", key.ConstBuffer(), ToString(value).ConstBuffer());

                m_settings.Add(key, value);
            }
        }

        return true;
    }

    void ProjectSettings::Save()
    {
        const File::Path projectSettingsJson = File::FixDependencies("Config/ProjectSettings.json");
        WriteJsonFile(projectSettingsJson, m_settings);
    }

    bool ProjectSettings::Has(const String& key) const
    {
        return m_settings.Has(key);
    }

    const String& ProjectSettings::Get(const String& key, const String& defaultStr) const
    {
        return Get<String>(key, defaultStr);
    }

    void ProjectSettings::Set(const String& key, const JsonData& value /* = JsonData()*/)
    {
        m_settings[key] = value;
    }

    void ProjectSettings::Set(const String& key, const char* value)
    {
        m_settings[key] = String(value);
    }

    void ProjectSettings::Erase(const String& key)
    {
        JPT_ASSERT(m_settings.Has(key), "ProjectSettings doesn't exist \"%s\"", key.ConstBuffer());
        m_settings.Erase(key);
    }
}
