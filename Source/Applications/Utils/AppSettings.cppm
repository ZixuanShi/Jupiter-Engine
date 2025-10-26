// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Settings;

import jpt.Concepts;
import jpt.String;
import jpt.TypeDefs;

import jpt.CommandLine;
import jpt.JsonData;
import jpt.ProjectSettings;

export namespace jpt
{
    template<typename T> requires (!IsCharArray<T> && !Enumerated<T>)
    [[nodiscard]] const T& SyncSettings(const String& key, const T& defaultValue)
    {
        CommandLine& commandLine = CommandLine::GetInstance();
        ProjectSettings& projectSettings = ProjectSettings::GetInstance();

        // Check Command Line first
        if (commandLine.Has(key))
        {
            return commandLine.Get<T>(key);
        }
        // Then check Project Settings
        else if (projectSettings.Has(key))
        {
            return projectSettings.Get<T>(key);
        }

        return defaultValue;
    }

    [[nodiscard]] const String& SyncSettings(const String& key, const String& defaultValue)
    {
        CommandLine& commandLine = CommandLine::GetInstance();
        ProjectSettings& projectSettings = ProjectSettings::GetInstance();

        // Check Command Line first
        if (commandLine.Has(key))
        {
            return commandLine.Get<String>(key);
        }
        // Then check Project Settings
        else if (projectSettings.Has(key))
        {
            return projectSettings.Get<String>(key);
        }

        return defaultValue;
    }

    template<Enumerated TEnum>
    [[nodiscard]] TEnum SyncSettings(const String& key, TEnum defaultValue)
    {
        CommandLine& commandLine = CommandLine::GetInstance();
        ProjectSettings& projectSettings = ProjectSettings::GetInstance();

        // Check Command Line first
        if (commandLine.Has(key))
        {
            return static_cast<TEnum>(commandLine.Get<int32>(key));
        }
        // Then check Project Settings
        else if (projectSettings.Has(key))
        {
            return static_cast<TEnum>(projectSettings.Get<int32>(key));
        }

        return defaultValue;
    }
}