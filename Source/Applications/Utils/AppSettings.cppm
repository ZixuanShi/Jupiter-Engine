// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.AppSettings;

import jpt.Concepts;
import jpt.String;
import jpt.TypeDefs;

import jpt.LaunchArgs;
import jpt.JsonData;
import jpt.ProjectSettings;

export namespace jpt
{
    // Priority: CommandLine > ProjectSettings > DefaultValue

    /** requires !IsCharArray<T> for not mixing raw char array with Strings for default value
        !jpt::Enumerated<T> for not mixing enum class with integers */
    template<typename T> requires (!IsCharArray<T> && !Enumerated<T>)
    [[nodiscard]] const T& GetSettings(const String& key, const T& defaultValue)
    {
        const LaunchArgs& launchArgs = LaunchArgs::GetInstance();
        const ProjectSettings& projectSettings = ProjectSettings::GetInstance();

        if (launchArgs.Has(key))
        {
            return launchArgs.Get<T>(key);
        }
        else if (projectSettings.Has(key))
        {
            return projectSettings.Get<T>(key);
        }

        return defaultValue;
    }

    /** Specialized for Strings input & output. 
        Work around for preventing compiler recognizing "foo" as const char[4] and wants to return it as const char[4] as well */
    [[nodiscard]] const String& GetSettings(const String& key, const String& defaultValue)
    {
        return GetSettings<String>(key, defaultValue);
    }

    /** Specialized for Jupiter Enums. 
        @note Should have enum template explicityly declared. i.e. GetSettings<FrameworkAPI>("frameworkAPI", FrameworkAPI::GLFW); */
    template<JptEnumerated TEnum, Enumerated TValue>
    [[nodiscard]] TEnum GetSettings(const String& key, TValue defaultValue)
    {
        const LaunchArgs& launchArgs = LaunchArgs::GetInstance();
        const ProjectSettings& projectSettings = ProjectSettings::GetInstance();

        // Enum could be stored as string or integer

        if (launchArgs.Has(key))
        {
            if (launchArgs.Is<String>(key))
            {
                const String& enumStr = launchArgs.Get<String>(key);
                return TEnum::FromName(enumStr);
            }
            else if (launchArgs.Is<int32>(key))
            {
                return static_cast<TValue>(launchArgs.Get<int32>(key));
            }
        }
        else if (projectSettings.Has(key))
        {
            if (projectSettings.Is<String>(key))
            {
                const String& enumStr = projectSettings.Get<String>(key);
                return TEnum::FromName(enumStr);
            }
            else if (projectSettings.Is<int32>(key))
            {
                return static_cast<TValue>(projectSettings.Get<int32>(key));
            }
        }

        return defaultValue;
    }

    /** Specialized for C++ Enums */
    template<Enumerated TValue>
    [[nodiscard]] TValue GetSettings(const String& key, TValue defaultValue)
    {
        const LaunchArgs& launchArgs = LaunchArgs::GetInstance();
        const ProjectSettings& projectSettings = ProjectSettings::GetInstance();

        // Enum could be stored as string or integer

        if (launchArgs.Has(key))
        {
            return static_cast<TValue>(launchArgs.Get<int32>(key));
        }
        else if (projectSettings.Has(key))
        {
            return static_cast<TValue>(projectSettings.Get<int32>(key));
        }

        return defaultValue;
    }

    void SetSettings()
    {
        // TODO
    }
}