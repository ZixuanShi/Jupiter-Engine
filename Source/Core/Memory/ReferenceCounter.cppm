// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ReferenceCounter;

import jpt.TypeDefs;

export namespace jpt_private
{
    class ReferenceCounter
    {
    private:
        uint32 m_strongRefs = 0;
        uint32 m_weakRefs = 0;

    public:
        constexpr ReferenceCounter() = default;
        constexpr ReferenceCounter(uint32 strongRefs, uint32 weakRefs) : m_strongRefs(strongRefs), m_weakRefs(weakRefs) {}

        constexpr void IncrementStrongRef()    { ++m_strongRefs; }
        constexpr void IncrementWeakRef()      { ++m_weakRefs;   }

        constexpr void DecrementStrongRef()    { --m_strongRefs; }
        constexpr void DecrementWeakRef()      { --m_weakRefs;   }

        constexpr uint32 GetStrongRefs() const { return m_strongRefs; }
        constexpr uint32 GetWeakRefs()   const { return m_weakRefs;   }

        constexpr bool HasAnyStrongRef() const { return m_strongRefs > 0; }
        constexpr bool HasAnyWeakRef()   const { return m_weakRefs   > 0; }
        constexpr bool HasAnyRef()       const { return HasAnyStrongRef() || HasAnyWeakRef(); }
    };
}