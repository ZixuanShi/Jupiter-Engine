// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"

export module jpt.ProjectSettings;

import jpt.JsonData;
import jpt.TypeTraits;

export namespace jpt
{
    /** Get & Set config at File::FixDependencies("Config/ProjectSettings.json")
        Source of truth. Runtime should use this instead of command line */
    class ProjectSettings
    {
    private:
        JsonMap m_settings;

    public:
        JPT_DECLARE_SINGLETON(ProjectSettings);

        bool Load();
        void Save();

        [[nodiscard]] bool Has(const String& key) const;

        /** @return true if the value associated with the key is of type T */
        template<ValidJsonType T>
        [[nodiscard]] bool Is(const String& key) const;

        template<typename T>
        [[nodiscard]] const T& Get(const String& key) const;

        /** requires !IsCharArray<T> for not mixing raw char array with Strings for default value
            !jpt::Enumerated<T> for not mixing enum class with integers */
        template<typename T> requires (!IsCharArray<T> && !Enumerated<T>)
        [[nodiscard]] const T& Get(const String& key, const T& defaultValue) const;

        /** Specialized for Jupiter Enum class */
        template<JptEnumerated TEnum, Enumerated TValue>
        [[nodiscard]] TEnum Get(const String& key, TValue defaultValue) const;

        /** Specialized for C++ Enum class */
        template<Enumerated TValue>
        [[nodiscard]] TValue Get(const String& key, TValue defaultValue) const;

        /** Specialized for Strings input & output. 
            Work around for preventing compiler recognizing "foo" as const char[4] and wants to return it as const char[4] as well */
        [[nodiscard]] const String& Get(const String& key, const String& defaultStr) const;

        void Set(const String& key, const JsonData& value = JsonData());
        void Set(const String& key, const char* value);

        template<Enumerated TEnum>
        void Set(const String& key, TEnum value);

        void Erase(const String& key);
    };

    template<ValidJsonType T>
    bool ProjectSettings::Is(const String& key) const
    {
        JPT_ASSERT(Has(key), "ProjectSettings doesn't exist \"%s\"", key.ConstBuffer());
        return m_settings[key].Is<T>();
    }

    template<typename T>
    const T& ProjectSettings::Get(const String& key) const
    {
        JPT_ASSERT(m_settings.Has(key));
        return m_settings[key].As<T>();
    }

    template<typename T> requires (!IsCharArray<T> && !Enumerated<T>)
    const T& ProjectSettings::Get(const String& key, const T& defaultValue) const
    {
        if (m_settings.Has(key))
        {
            return m_settings[key].As<T>();
        }

        return defaultValue;
    }

    template<JptEnumerated TEnum, Enumerated TValue>
    TEnum ProjectSettings::Get(const String& key, TValue defaultValue) const
    {
        if (m_settings.Has(key))
        {
            if (m_settings[key].Is<String>())
            {
                const String& enumStr = m_settings[key].As<String>();
                return TEnum::FromName(enumStr);
            }
            else if (m_settings[key].Is<int32>())
            {
                return static_cast<TValue>(m_settings[key].As<int32>());
            }
        }

        return defaultValue;
    }

    template<Enumerated TValue>
    TValue ProjectSettings::Get(const String& key, TValue defaultValue) const
    {
        if (m_settings.Has(key))
        {
            return static_cast<TValue>(m_settings[key].As<int32>());
        }

        return defaultValue;
    }

    template<Enumerated TEnum>
    void ProjectSettings::Set(const String& key, TEnum value)
    {
        m_settings[key] = static_cast<int32>(value);
    }
}