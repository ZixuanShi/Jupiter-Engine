// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
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

import jpt.TimeTypeDefs;

export namespace jpt
{
    class EventManager
    {
        JPT_DECLARE_SINGLETON(EventManager);

    private:
        struct Handler
        {
            Function<void(const Event&)> func; /**< Function to be called when an event is sent. Could be global or member or local lambda */
            const void* pOwner = nullptr;       /**< class instance if func is it's member function. Function address if func is global or lambda */
        };

        /** Queued events to be sent later */
        struct QueueItem
        {
            Event* pEvent = nullptr;         /**< Event to be sent */
            Id eventId = kInvalidValue<Id>;     /**< Id of the event when Queue() called. Used for getting the right handlers */
            TimePrecision m_timer = 0.0;     /**< Timer to delay the event. 0.0 means next frame */
        };

    private:
        using Handlers     = DynamicArray<Handler>;   /**< List of functions to be called when an event is sent */
        using HandlersMap  = HashMap<Id, Handlers>;   /**< Key: Event Id. Value: The handlers that are registered to listen to this event */

    private:
        HandlersMap m_handlersMap;                /**< Map from event Ids to handlers */
        DynamicArray<QueueItem> m_eventQueue;    /**< Queue of events to be sent later */

    public:
        /** Register a member function to event */
        template<typename TEvent, typename TListener>
        void Register(TListener* pListener, void(TListener::* pMemberFunction)(const TEvent&));

        /** Register a global function or lambda to event */
        template<typename TEvent, typename THandlerFunc>
        void Register(THandlerFunc&& func);

        /** Unregister a listener from an event */
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

        /** Process all events in the queue */
        void Update(TimePrecision deltaSeconds);

        /** Clears all remaining events */
        void Terminate();

    private:
        template<typename TEvent>       Handlers& GetHandlers();
        template<typename TEvent> const Handlers& GetHandlers() const;
    };

    template<typename TEvent, typename TListener>
    void EventManager::Register(TListener* pListener, void(TListener::* pMemberFunction)(const TEvent&))
    {
        auto handlerFunc = [pListener, pMemberFunction](const Event& event)
            {
                (pListener->*pMemberFunction)(static_cast<const TEvent&>(event));
            };

        GetHandlers<TEvent>().EmplaceBack(handlerFunc, pListener);
    }

   template<typename TEvent, typename THandlerFunc>
   void EventManager::Register(THandlerFunc&& func)
   {
       // Lambda wrapper to call the function
       auto handlerFunc = [func](const Event& event)
           {
               func(static_cast<const TEvent&>(event));
           };

       // Get the address of the function
       void* address = nullptr;
       if constexpr (std::is_function_v<TRemovePointer<TRemoveReference<THandlerFunc>>>)
       {
           // Global function
           using FuncPtr = void(*)(const TEvent&);
           FuncPtr fptr = func;
           address = reinterpret_cast<void*>(fptr);
       }
       else
       {
           // Lambda or other callable
           address = reinterpret_cast<void*>(std::addressof(func));
       }

       GetHandlers<TEvent>().EmplaceBack(handlerFunc, address);
   }

    template<typename TEvent, typename TListener>
    void EventManager::Unregister(TListener* pListener)
    {
        Handlers& handlers = GetHandlers<TEvent>();

        for (int32 i = static_cast<int32>(handlers.Count()) - 1; i >= 0; --i)
        {
            if (handlers[i].pOwner == pListener)
            {
                handlers.Erase(i);
            }
        }
    }

    template<typename TEvent>
    void EventManager::UnregisterAll()
    {
        GetHandlers<TEvent>().Clear();
    }

    template<typename TEvent>
    void EventManager::Send(const TEvent& event)
    {
        for (const Handler& handler : GetHandlers<TEvent>())
        {
            handler.func(event);
        }
    }

    template<typename TEvent>
    void EventManager::Queue(const TEvent& event, TimePrecision timer /*= 0.0*/)
    {
        m_eventQueue.EmplaceBack(Allocator<TEvent>::New(event), TypeRegistry::GetId<TEvent>(), timer);
    }

    void EventManager::Update(TimePrecision deltaSeconds)
    {
        for (auto itr = m_eventQueue.begin(); itr != m_eventQueue.end();)
        {
            QueueItem& item = *itr;
            item.m_timer -= deltaSeconds;

            if (item.m_timer <= 0.0)
            {
                const Handlers& handlers = m_handlersMap[item.eventId];
                for (const Handler& handlerData : handlers)
                {
                    handlerData.func(*item.pEvent);
                }

                Allocator<Event>::Delete(item.pEvent);
                item.pEvent = nullptr;

                itr = m_eventQueue.Erase(itr);
            }
            else
            {
                ++itr;
            }
        }
    }

    void EventManager::Terminate()
    {
        for (QueueItem& item : m_eventQueue)
        {
            Allocator<Event>::Delete(item.pEvent);
            item.pEvent = nullptr;
        }

        m_eventQueue.Clear();
    }

    template<typename TEvent>
    bool EventManager::IsListening(const void* pListener) const
    {
        for (const Handler& handler : GetHandlers<TEvent>())
        {
            if (handler.pOwner == pListener)
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
}