// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module jpt.Event.Manager;

import jpt.DynamicArray;
import jpt.HashMap;
import jpt.Function;
import jpt.Pair;
import jpt.TypeRegistry;
import jpt.Utilities;
import jpt.Event;

export namespace jpt
{
	class EventManager
	{
		JPT_SINGLETON_DECLARATION(EventManager);

	private:
		using HandlerFunc = Function<void(const Event&)>;	/**< Function to be called when an event is sent */

		struct Handler
		{
			HandlerFunc func;	    /**< Function to be called when an event is sent. Could be global or member or local lambda */
			const void* pOwner;		/**< valid if handler is member function of pOwner. nullptr if is global or lambda */
		};

		struct QueueItem
		{
			Event event;
			TypeRegistry::TypeId eventId;
		};

		using Handlers     = DynamicArray<Handler>;                     /**< List of functions to be called when an event is sent */
		using HandlersMap  = HashMap<TypeRegistry::TypeId, Handlers>;   /**< Key: Event Id. Value: The handlers that are registered to listen to this event */

	private:
		HandlersMap m_handlersMap;		/**< Map of event Ids to handlers */
		DynamicArray<QueueItem> m_eventQueue;	/**< Queue of events to be sent */

	public:
		/** Register a member function to event */
		template<typename TEvent, typename TListener>
		void Register(TListener* pListener, void(TListener::* pMemberFunction)(const TEvent&));

		/** Register a global function or lambda to event */
		template<typename TEvent, typename THandlerFunc>
		void Register(THandlerFunc&& handlerFunc);

		/** Unregister a listener from an event */
		template<typename TEvent, typename TListener>
		void Unregister(TListener* pListener);

		/** Unregister all listeners from an event */
		template<typename TEvent>
		void UnregisterAll();

		/** Send an event to all listeners */
		template<typename TEvent>
		void Send(const TEvent& event);

		/** Queue an event to be sent */
		template<typename TEvent>
		void Queue(const TEvent& event);

		/** Process all events in the queue */
		void SendQueuedEvents();

	private:
		template<typename TEvent>
		Handlers& GetHandlers();
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
	   auto handlerFunc = [func](const Event& event)
		   {
			   func(static_cast<const TEvent&>(event));
		   };

	   GetHandlers<TEvent>().EmplaceBack(handlerFunc, nullptr);
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
		// Send the event to all registered handlers
		for (const Handler& handlerData : GetHandlers<TEvent>())
		{
			handlerData.func(event);
		}
	}

	template<typename TEvent>
	void EventManager::Queue(const TEvent& event)
	{
		m_eventQueue.EmplaceBack(event, TypeRegistry::Id<TEvent>());
	}

	template<typename TEvent>
	EventManager::Handlers& EventManager::GetHandlers()
	{
		const TypeRegistry::TypeId eventId = TypeRegistry::Id<TEvent>();
		return m_handlersMap[eventId];
	}

	void EventManager::SendQueuedEvents()
	{
		for (QueueItem& item : m_eventQueue)
		{
			Handlers& handlers = m_handlersMap[item.eventId];

			for (const Handler& handlerData : handlers)
			{
				handlerData.func(item.event);
			}
		}

		m_eventQueue.Clear();
	}
}