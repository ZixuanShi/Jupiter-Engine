// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"
#include "Core/Minimal/Utilities.h"
#include "Debugging/Logger.h"

#include <type_traits>
#include <memory>

export module jpt.EventManager;

import jpt.Constants;
import jpt.TypeTraits;
import jpt.TypeRegistry;
import jpt.Utilities;

import jpt.DynamicArray;
import jpt.HashMap;

import jpt.Function;
import jpt.Event;

export namespace jpt
{
    // Handle for event registration - used to unregister later
    struct EventHandle
    {
        Id eventId = kInvalidId;
        Index handlerIndex = kInvalidIndex;

        constexpr bool IsValid() const { return eventId != kInvalidId && handlerIndex != kInvalidIndex; }
        constexpr void Invalidate() { eventId = kInvalidValue<Id>; handlerIndex = kInvalidIndex; }
    };

    class EventManager
    {
        JPT_DECLARE_SINGLETON(EventManager);

    private:
        struct Handler
        {
            Function<void(const Event&)> func;  /**< Function to be called when an event is sent */
            const void* pOwner = nullptr;       /**< Class instance if func is member function, otherwise nullptr */
            size_t handleId = kInvalidValue<size_t>;  /**< Unique ID for this handler */
            bool isActive = true;               /**< Whether this handler is active (for lazy deletion) */
        };

        /** Queued events to be sent later */
        struct QueueItem
        {
            Event* pEvent = nullptr;
            Id eventId = kInvalidValue<Id>;
            TimePrecision m_timer = 0.0;
        };

    private:
        using Handlers = DynamicArray<Handler>;
        using HandlersMap = HashMap<Id, Handlers>;

    private:
        HandlersMap m_handlersMap;
        DynamicArray<QueueItem> m_eventQueue;
        Index m_nextHandleId = 0;  /**< Counter for generating unique handle IDs */

    public:
        /** Register a member function to event - returns handle for unregistration */
        template<typename TEvent, typename TListener>
        EventHandle Register(TListener* pListener, void(TListener::* pMemberFunction)(const TEvent&));

        /** Register a global function or lambda to event - returns handle for unregistration */
        template<typename TEvent, typename THandlerFunc>
        EventHandle Register(THandlerFunc&& func);

        /** Unregister using handle returned from Register */
        void Unregister(EventHandle handle);

        /** Unregister a listener from an event (works for member functions only) */
        template<typename TEvent, typename TListener>
        void Unregister(TListener* pListener);

        /** Unregister all listeners from an event */
        template<typename TEvent>
        void UnregisterAll();

        /** Send an event to all listeners now at current frame */
        template<typename TEvent>
        void Send(const TEvent& event);

        /** Queue an event to be sent later */
        template<typename TEvent>
        void Queue(const TEvent& event, TimePrecision timer = 0.0);

        /** @return true if Listener is already listening to an event, false if not */
        template<typename TEvent>
        bool IsListening(const void* pListener) const;

        /** @return true if handle is still valid and registered */
        bool IsListening(EventHandle handle) const;

        /** Process all events in the queue */
        void Update(TimePrecision deltaSeconds);

        /** Clears all remaining events */
        void Terminate();

    private:
        template<typename TEvent>       Handlers& GetHandlers();
        template<typename TEvent> const Handlers& GetHandlers() const;

        Index GenerateHandleId() { return m_nextHandleId++; }
        void CleanupInactiveHandlers(Handlers& handlers);
    };

    template<typename TEvent, typename TListener>
    EventHandle EventManager::Register(TListener* pListener, void(TListener::* pMemberFunction)(const TEvent&))
    {
        auto handlerFunc = [pListener, pMemberFunction](const Event& event)
            {
                (pListener->*pMemberFunction)(static_cast<const TEvent&>(event));
            };

        Handlers& handlers = GetHandlers<TEvent>();
        const Index handleId = GenerateHandleId();

        handlers.EmplaceBack();
        Handler& handler = handlers.Back();
        handler.func = Move(handlerFunc);
        handler.pOwner = pListener;
        handler.handleId = handleId;
        handler.isActive = true;

        EventHandle handle;
        handle.eventId = TypeRegistry::GetId<TEvent>();
        handle.handlerIndex = handleId;
        return handle;
    }

    template<typename TEvent, typename THandlerFunc>
    EventHandle EventManager::Register(THandlerFunc&& func)
    {
        // Capture the function by move/forward
        auto handlerFunc = [capturedFunc = Forward<THandlerFunc>(func)](const Event& event)
            {
                capturedFunc(static_cast<const TEvent&>(event));
            };

        Handlers& handlers = GetHandlers<TEvent>();
        const Index handleId = GenerateHandleId();

        handlers.EmplaceBack();
        Handler& handler = handlers.Back();
        handler.func = Move(handlerFunc);
        handler.pOwner = nullptr;  // No owner for lambdas/global functions
        handler.handleId = handleId;
        handler.isActive = true;

        EventHandle handle;
        handle.eventId = TypeRegistry::GetId<TEvent>();
        handle.handlerIndex = handleId;
        return handle;
    }

    inline void EventManager::Unregister(EventHandle handle)
    {
        if (!handle.IsValid())
        {
            return;
        }

        auto itr = m_handlersMap.Find(handle.eventId);
        if (itr == m_handlersMap.end())
        {
            return;
        }

        Handlers& handlers = itr->second;

        // Mark handler as inactive (lazy deletion to avoid invalidating iterators during Send)
        for (Handler& handler : handlers)
        {
            if (handler.handleId == handle.handlerIndex)
            {
                handler.isActive = false;
                break;
            }
        }

        // Clean up inactive handlers
        CleanupInactiveHandlers(handlers);
    }

    template<typename TEvent, typename TListener>
    void EventManager::Unregister(TListener* pListener)
    {
        Handlers& handlers = GetHandlers<TEvent>();

        for (Handler& handler : handlers)
        {
            if (handler.pOwner == pListener)
            {
                handler.isActive = false;
            }
        }

        CleanupInactiveHandlers(handlers);
    }

    template<typename TEvent>
    void EventManager::UnregisterAll()
    {
        GetHandlers<TEvent>().Clear();
    }

    template<typename TEvent>
    void EventManager::Send(const TEvent& event)
    {
        Handlers& handlers = GetHandlers<TEvent>();

        for (const Handler& handler : handlers)
        {
            if (handler.isActive)
            {
                handler.func(event);
            }
        }

        // Clean up after sending
        CleanupInactiveHandlers(handlers);
    }

    template<typename TEvent>
    void EventManager::Queue(const TEvent& event, TimePrecision timer /*= 0.0*/)
    {
        m_eventQueue.EmplaceBack(JPT_NEW(TEvent, event), TypeRegistry::GetId<TEvent>(), timer);
    }

    inline void EventManager::Update(TimePrecision deltaSeconds)
    {
        for (auto itr = m_eventQueue.begin(); itr != m_eventQueue.end();)
        {
            QueueItem& item = *itr;
            item.m_timer -= deltaSeconds;

            if (item.m_timer <= 0.0)
            {
                auto handlersItr = m_handlersMap.Find(item.eventId);
                if (handlersItr != m_handlersMap.end())
                {
                    const Handlers& handlers = handlersItr->second;
                    for (const Handler& handler : handlers)
                    {
                        if (handler.isActive)
                        {
                            handler.func(*item.pEvent);
                        }
                    }
                }

                JPT_DELETE(item.pEvent);
                item.pEvent = nullptr;

                itr = m_eventQueue.Erase(itr);
            }
            else
            {
                ++itr;
            }
        }
    }

    inline void EventManager::Terminate()
    {
        for (QueueItem& item : m_eventQueue)
        {
            JPT_DELETE(item.pEvent);
            item.pEvent = nullptr;
        }

        m_eventQueue.Clear();
    }

    template<typename TEvent>
    bool EventManager::IsListening(const void* pListener) const
    {
        const Handlers& handlers = GetHandlers<TEvent>();
        for (const Handler& handler : handlers)
        {
            if (handler.isActive && handler.pOwner == pListener)
            {
                return true;
            }
        }

        return false;
    }

    inline bool EventManager::IsListening(EventHandle handle) const
    {
        if (!handle.IsValid())
        {
            return false;
        }

        auto itr = m_handlersMap.Find(handle.eventId);
        if (itr == m_handlersMap.end())
        {
            return false;
        }

        const Handlers& handlers = itr->second;
        for (const Handler& handler : handlers)
        {
            if (handler.handleId == handle.handlerIndex && handler.isActive)
            {
                return true;
            }
        }

        return false;
    }

    template<typename TEvent>
    EventManager::Handlers& EventManager::GetHandlers()
    {
        const Id eventId = TypeRegistry::GetId<TEvent>();
        return m_handlersMap[eventId];
    }

    template<typename TEvent>
    const EventManager::Handlers& EventManager::GetHandlers() const
    {
        const Id eventId = TypeRegistry::GetId<TEvent>();
        return m_handlersMap[eventId];
    }

    inline void EventManager::CleanupInactiveHandlers(Handlers& handlers)
    {
        // Remove all inactive handlers
        for (int32 i = static_cast<int32>(handlers.Count()) - 1; i >= 0; --i)
        {
            if (!handlers[i].isActive)
            {
                handlers.Erase(i);
            }
        }
    }
}