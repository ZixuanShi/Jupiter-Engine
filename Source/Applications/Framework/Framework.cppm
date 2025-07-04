// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Framework;

import jpt.TypeDefs;
import jpt.TimeTypeDefs;

export namespace jpt
{
    class Framework
    {
    public:
        virtual ~Framework() = default;

        virtual bool PreInit() { return true; }
        virtual bool Init() { return true; }
        virtual void Update(TimePrecision) {}
        virtual void Terminate() {}

        virtual const char** GetRequiredExtensions([[maybe_unused]] uint32& extensionCount) { return nullptr; }
    };
}