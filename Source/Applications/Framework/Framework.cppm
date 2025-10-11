// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Framework;

import jpt.Framework_Enums;
import jpt.TypeDefs;

export namespace jpt
{
    class Framework
    {
    protected:
        FrameworkAPI m_api = FrameworkAPI::Unknown;

    public:
        virtual ~Framework() = default;

        virtual bool PreInit();
        virtual bool Init() { return true; }
        virtual void Update(TimePrecision) {}
        virtual void Terminate() {}

        virtual const char** GetRequiredExtensions([[maybe_unused]] uint32& extensionCount) { return nullptr; }

        FrameworkAPI GetAPI() const { return m_api; }
    };
}
