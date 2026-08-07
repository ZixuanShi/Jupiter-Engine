// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EventDispatcher;

import jpt.Constants;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** Multicast, immediate. A handler may Add or Remove during Dispatch, including itself. */
    template<typename TEvent>
    class EventDispatcher
    {
    public:
        using Handler = std::function<void(const TEvent&)>;
        using Handle  = uint32;

    private:
        struct Slot
        {
            Handle  handle = kInvalid<Handle>;
            Handler handler;
        };

        std::deque<Slot> m_slots;   // Not vector: growing must not move a handler mid-Dispatch.
        Handle m_nextHandle = 0;
        uint32 m_dispatchDepth = 0;

    public:
        Handle Add(Handler handler);

        template<typename TObject>
        Handle Add(TObject* pObject, void (TObject::* pMemberFunction)(const TEvent&));
        
        void Remove(Handle handle);

        void Dispatch(const TEvent& event);

        [[nodiscard]] usize GetCount() const noexcept;
    };

    template<typename TEvent>
    typename EventDispatcher<TEvent>::Handle EventDispatcher<TEvent>::Add(Handler handler)
    {
        const Handle handle = m_nextHandle++;

        // Never mid-dispatch: the reused slot could be the one executing.
        if (m_dispatchDepth == 0)
        {
            for (Slot& slot : m_slots)
            {
                if (slot.handle == kInvalid<Handle>)
                {
                    slot.handle  = handle;
                    slot.handler = std::move(handler);
                    return handle;
                }
            }
        }

        m_slots.emplace_back(handle, std::move(handler));
        return handle;
    }

    template<typename TEvent>
    template<typename TObject>
    typename EventDispatcher<TEvent>::Handle EventDispatcher<TEvent>::Add(TObject* pObject, void (TObject::* pMemberFunction)(const TEvent&))
    {
        return Add([pObject, pMemberFunction](const TEvent& event) { (pObject->*pMemberFunction)(event); });
    }

    template<typename TEvent>
    void EventDispatcher<TEvent>::Remove(Handle handle)
    {
        for (Slot& slot : m_slots)
        {
            if (slot.handle == handle)
            {
                slot.handle = kInvalid<Handle>;

                // Destroying it here would be fatal if a handler is removing itself.
                if (m_dispatchDepth == 0)
                {
                    slot.handler = nullptr;
                }
                return;
            }
        }
    }

    template<typename TEvent>
    void EventDispatcher<TEvent>::Dispatch(const TEvent& event)
    {
        const usize count = m_slots.size();   // A handler added now must not see this event.

        ++m_dispatchDepth;
        for (usize i = 0; i < count; ++i)
        {
            if (m_slots[i].handle != kInvalid<Handle>)
            {
                m_slots[i].handler(event);
            }
        }
        --m_dispatchDepth;
    }

    template<typename TEvent>
    usize EventDispatcher<TEvent>::GetCount() const noexcept
    {
        usize count = 0;
        for (const Slot& slot : m_slots)
        {
            if (slot.handle != kInvalid<Handle>)
            {
                ++count;
            }
        }
        return count;
    }
}
