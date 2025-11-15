// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EventHandle;

import jpt.Constants;
import jpt.Event;
import jpt.TypeDefs;

import jpt_private.EventHelpers;

export namespace jpt
{
    /** The handle returned of EventManager::Register(). Used for unregister and check is listening */
    // Could be moved to Event.cppm
    struct EventHandle
    {
        Id eventId    = kInvalidId;
        Id functionId = kInvalidId;
    };
}