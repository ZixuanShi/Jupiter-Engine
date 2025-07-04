// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event;

export namespace jpt
{
    /** Base class for all events */
    class Event
    {
    public:
        virtual ~Event() = default;
    };
}