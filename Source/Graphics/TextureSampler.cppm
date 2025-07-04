// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TextureSampler;

import jpt.Any;
import jpt.DynamicArray;

export namespace jpt
{
    class TextureSampler
    {
    public:
        virtual ~TextureSampler() = default;

    public:
        virtual bool Init() { return true; }
        virtual void Terminate() {}
    };
}