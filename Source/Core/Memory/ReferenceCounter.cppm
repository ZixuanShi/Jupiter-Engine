// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ReferenceCounter;

import jpt.TypeDefs;

export namespace jpt_private
{
    class ReferenceCounter
    {
    private:
        uint32 m_sharedRefs = 0;
        uint32 m_weakRefs = 0;

    public:
        constexpr ReferenceCounter() = default;
        constexpr ReferenceCounter(uint32 strongRefs, uint32 weakRefs) : m_sharedRefs(strongRefs), m_weakRefs(weakRefs) {}

        constexpr void IncrementSharedRef()    { ++m_sharedRefs; }
        constexpr void IncrementWeakRef()      { ++m_weakRefs;   }

        constexpr void DecrementSharedRef()    { --m_sharedRefs; }
        constexpr void DecrementWeakRef()      { --m_weakRefs;   }

        constexpr uint32 GetSharedRefs() const { return m_sharedRefs; }
        constexpr uint32 GetWeakRefs()   const { return m_weakRefs;   }

        constexpr bool HasAnySharedRef() const { return m_sharedRefs > 0; }
        constexpr bool HasAnyWeakRef()   const { return m_weakRefs   > 0; }
        constexpr bool HasAnyRef()       const { return HasAnySharedRef() || HasAnyWeakRef(); }
    };
}