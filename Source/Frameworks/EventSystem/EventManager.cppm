// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"
#include "Core/Minimal/Utilities.h"
#include "Debugging/Logger.h"

export module jpt.EventManager;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.Function;
import jpt.HashMap;
import jpt.TypeTraits;
import jpt.TypeRegistry;
import jpt.Utilities;

import jpt.Event;
import jpt.EventHandle;

import jpt_private.EventHelpers;

export namespace jpt
{
    class EventManager
    {
        JPT_DECLARE_SINGLETON(EventManager);
        
    private:
        jpt_private::FunctionsMap m_functionsMap;  /**< Map from event Ids to handlers */
        jpt_private::EventQueue   m_eventQueue;    /**< Queue of events to be sent later */

        Id m_handleId = 0;  /**< Counter for generating unique handle IDs */

    public:
        /** Register a member function to event */
        template<typename TEvent, typename TListener>
        EventHandle Register(TListener* pListener, void(TListener::* pMemberFunction)(const TEvent&));

        /** Register a global function or lambda to event */
        template<typename TEvent, typename THandlerFunc>
        EventHandle Register(THandlerFunc&& func);

        /** Unregister a listener from an event
            @param pListener    Cannot be a lambda. */
        template<typename TEvent, Functional TListener>
        void Unregister(TListener* pListener);

        void Unregister(EventHandle eventHandle);

        /** Unregister all listeners from an event */
        template<typename TEvent>
        void UnregisterAll();

        /** Send an event to all listeners now at current frame */
        template<typename TEvent>
        void Send(const TEvent& event);

        /** Queue an event to be sent later */
        template<typename TEvent>
        void Queue(const TEvent& event, TimePrecision timer = 0.0);

        /** @return true if handle is listening to an event, false if not */
        template<typename TEvent, Functional TListener>
        bool IsListening(const TListener* pListener) const;

        bool IsListening(EventHandle eventHandle) const;

        /** Process all events in the queue */
        void Update(TimePrecision deltaSeconds);

        /** Clears all remaining events */
        void Terminate();

    private:
        template<typename TEvent>       jpt_private::EventFunctions& GetFunctions();
        template<typename TEvent> const jpt_private::EventFunctions& GetFunctions() const;
    };

    template<typename TEvent, typename TListener>
    EventHandle EventManager::Register(TListener* pListener, void(TListener::* pMemberFunction)(const TEvent&))
    {
        auto handlerFunc = [pListener, pMemberFunction](const Event& event)
            {
                (pListener->*pMemberFunction)(static_cast<const TEvent&>(event));
            };

        jpt_private::EventFunction& eventFunction = GetFunctions<TEvent>().EmplaceBack();
        eventFunction.func     = Move(handlerFunc);
        eventFunction.pContext = pListener;
        eventFunction.Id       = m_handleId++;
        
        const Id eventId = TypeRegistry::GetId<TEvent>();
        return EventHandle(eventId, eventFunction.Id);
    }

   template<typename TEvent, typename THandlerFunc>
   EventHandle EventManager::Register(THandlerFunc&& func)
   {
       // Lambda wrapper to call the function
        auto handlerFunc = [func](const Event& event)
           {
               func(static_cast<const TEvent&>(event));
           };

        jpt_private::EventFunction& eventFunction = GetFunctions<TEvent>().EmplaceBack();
        eventFunction.func     = Move(handlerFunc);
        eventFunction.Id       = m_handleId++;

        if constexpr (IsFunction<TRemovePointer<TRemoveReference<THandlerFunc>>>)
        {
            // Global function
            using FuncPtr = void(*)(const TEvent&);
            FuncPtr fptr = func;
            eventFunction.pContext = reinterpret_cast<void*>(fptr);
        }
        else
        {
            // Lambda has no context
            eventFunction.pContext = nullptr; 
        }
        
        const Id eventId = TypeRegistry::GetId<TEvent>();
        return EventHandle(eventId, eventFunction.Id);
   }

   template<typename TEvent, Functional TListener>
   void EventManager::Unregister(TListener* pListener)
   {
       jpt_private::EventFunctions& functions = GetFunctions<TEvent>();

       for (int32 i = static_cast<int32>(functions.Count()) - 1; i >= 0; --i)
       {
           if (functions[i].pContext == pListener)
           {
               functions.Erase(i);
           }
       }
   }

    void EventManager::Unregister(EventHandle eventHandle)
    {
        jpt_private::EventFunctions& functions = m_functionsMap[eventHandle.eventId];

        for (int32 i = static_cast<int32>(functions.Count()) - 1; i >= 0; --i)
        {
            if (functions[i].Id == eventHandle.functionId)
            {
                functions.Erase(i);
            }
        }
    }

    template<typename TEvent>
    void EventManager::UnregisterAll()
    {
        GetFunctions<TEvent>().Clear();
    }

    template<typename TEvent>
    void EventManager::Send(const TEvent& event)
    {
        for (const jpt_private::EventFunction& function : GetFunctions<TEvent>())
        {
            function.func(event);
        }
    }

    template<typename TEvent>
    void EventManager::Queue(const TEvent& event, TimePrecision timer /*= 0.0*/)
    {
        const Id eventId = TypeRegistry::GetId<TEvent>();

        m_eventQueue.EmplaceBack(JPT_NEW(TEvent, event), eventId, timer);
    }

    template<typename TEvent, Functional TListener>
    bool EventManager::IsListening(const TListener* pListener) const
    {
        for (const jpt_private::EventFunction& function : GetFunctions<TEvent>())
        {
            if (function.pContext == pListener)
            {
                return true;
            }
        }

        return false;
    }

    bool EventManager::IsListening(EventHandle eventHandle) const
    {
        for (const jpt_private::EventFunction& function : m_functionsMap[eventHandle.eventId])
        {
            if (function.Id == eventHandle.functionId)
            {
                return true;
            }
        }

        return false;
    }

    void EventManager::Update(TimePrecision deltaSeconds)
    {
        for (auto itr = m_eventQueue.begin(); itr != m_eventQueue.end();)
        {
            jpt_private::PendingEvent& item = *itr;
            item.m_timer -= deltaSeconds;

            if (item.m_timer <= 0.0)
            {
                const jpt_private::EventFunctions& functions = m_functionsMap[item.eventId];
                for (const jpt_private::EventFunction& function : functions)
                {
                    function.func(*item.pEvent);
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

    void EventManager::Terminate()
    {
        for (jpt_private::PendingEvent& item : m_eventQueue)
        {
            JPT_DELETE(item.pEvent);
            item.pEvent = nullptr;
        }

        m_eventQueue.Clear();
    }

    template<typename TEvent>
    jpt_private::EventFunctions& EventManager::GetFunctions()
    {
        const Id eventId = TypeRegistry::GetId<TEvent>();

        return m_functionsMap[eventId];
    }

    template<typename TEvent>
    const jpt_private::EventFunctions& EventManager::GetFunctions() const
    {
        const Id eventId = TypeRegistry::GetId<TEvent>();

        return m_functionsMap[eventId];
    }
}