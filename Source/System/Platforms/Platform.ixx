// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Platform;

import jpt.TypeDefs;

export namespace jpt
{
    class Platform
    {
    public:
        virtual ~Platform() = default;

    public:
        virtual bool PreInit() { return true; }
        virtual bool Init() { return true; }
        virtual void Update([[maybe_unused]] TimePrecision deltaSeconds) {}
        virtual void Shutdown() {}
    };
}