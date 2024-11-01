// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <type_traits>
#include <memory>

export module jpt.Event.Manager;

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
	class EventManager
	{
		JPT_SINGLETON_DECLARATION(EventManager);

	private:
		using HandlerFunc = Function<void(const Event&)>;	/**< Type of handler Function */

		struct Handler
		{
			HandlerFunc func;	            /**< Function to be called when an event is sent. Could be global or member or local lambda */
			const void* pOwner = nullptr;	/**< class instance if func is it's member function. Function address if func is global or lambda */
		};

		/** Queue item to store event and it's type Id and send later */
		struct QueueItem
		{
			Event* pEvent = nullptr;
			TypeRegistry::TypeId eventId = kInvalidValue<TypeRegistry::TypeId>;
		};

		using Handlers     = DynamicArray<Handler>;                     /**< List of functions to be called when an event is sent */
		using HandlersMap  = HashMap<TypeRegistry::TypeId, Handlers>;   /**< Key: Event Id. Value: The handlers that are registered to listen to this event */

	private:
		HandlersMap m_handlersMap;		/**< Map of event Ids to handlers */
		DynamicArray<QueueItem> m_eventQueue;	/**< Queue of events to be sent later */

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
		void Queue(const TEvent& event);

		/** Process all events in the queue */
		void SendQueuedEvents();

		/** @return true if Listener is already listening to an event, false if not */
		template<typename TEvent>
		bool IsListening(const void* pListener) const;

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
		// Send the event to all registered handlers
		for (const Handler& handler : GetHandlers<TEvent>())
		{
			handler.func(event);
		}
	}

	template<typename TEvent>
	void EventManager::Queue(const TEvent& event)
	{
		QueueItem item;
		item.pEvent = new TEvent(event);
		item.eventId = TypeRegistry::Id<TEvent>();

		m_eventQueue.EmplaceBack(item);
	}

	void EventManager::SendQueuedEvents()
	{
		for (QueueItem& item : m_eventQueue)
		{
			Handlers& handlers = m_handlersMap[item.eventId];
			for (Handler& handlerData : handlers)
			{
				handlerData.func(*item.pEvent);

				delete item.pEvent;
				item.pEvent = nullptr;
			}
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
		const TypeRegistry::TypeId eventId = TypeRegistry::Id<TEvent>();
		return m_handlersMap[eventId];
	}

	template<typename TEvent>
	const EventManager::Handlers& EventManager::GetHandlers() const
	{
		const TypeRegistry::TypeId eventId = TypeRegistry::Id<TEvent>();
		return m_handlersMap[eventId];
	}
}