// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.EventHelpers;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.Event;
import jpt.Function;
import jpt.HashMap;
import jpt.TypeDefs;

export namespace jpt_private
{
    /** The Function to call when an event is triggered */
    struct EventFunction
    {
        // The function to call
        jpt::Function<void(const jpt::Event&)> func;  /**< Could be global or member or local lambda */
        const void* pContext = nullptr;               /**< class instance if func is it's member function. Memory address if func is a global function. nullptr if func is lambda */
        
        // Search and identification
        size_t Id = kInvalidId; /**< Unique ID for this handler */
        bool isActive = true;   /**< Whether this handler is active (for lazy deletion) */
    };

    /** Queued event to be sent later */
    struct PendingEvent
    {
        jpt::Event* pEvent = nullptr;   /**< Event to be sent */
        Id eventId = kInvalidId;        /**< Id of the event. Used for getting the right EventFunction */
        TimePrecision m_timer = 0.0;    /**< Timer to delay the event. 0.0 means next frame */
    };

    using EventFunctions = jpt::DynamicArray<EventFunction>;  /**< Array of functions to be called when an event is triggered */
    using FunctionsMap   = jpt::HashMap<Id, EventFunctions>;  /**< Key: Event Id. Value: The functions array that are registered to this event */
    using EventQueue     = jpt::DynamicArray<PendingEvent>;   /**< Queued events to be sent later */
}