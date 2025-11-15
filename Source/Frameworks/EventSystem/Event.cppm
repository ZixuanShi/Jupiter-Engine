// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event;

import jpt.TypeDefs;

export namespace jpt
{
    /** Base class for all events */
    class Event
    {
    public:
        using Id = Id;

    public:
        virtual ~Event() = default;
    };
}