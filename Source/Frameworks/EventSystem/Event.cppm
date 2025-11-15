// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Event;

import jpt.Constants;
import jpt.TypeDefs;

export namespace jpt
{
    /** Base class for all events */
    class Event
    {
    public:
        virtual ~Event() = default;
    };

    /** The handle returned of EventManager::Register(). Used for unregister and check is listening */
    struct EventHandle
    {
        Id eventId    = kInvalidId;
        Id functionId = kInvalidId;
    };
}