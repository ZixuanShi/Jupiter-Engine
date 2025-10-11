// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Framework_GLFW;

import jpt.Framework;
import jpt.TypeDefs;

export namespace jpt
{
    class Framework_GLFW final : public Framework
    {
        using Super = Framework;

    public:
        virtual bool PreInit() override;
        virtual void Update(TimePrecision deltaSeconds) override;
        virtual void Terminate() override;

        virtual const char** GetRequiredExtensions(uint32& extensionCount) override;
    };
}