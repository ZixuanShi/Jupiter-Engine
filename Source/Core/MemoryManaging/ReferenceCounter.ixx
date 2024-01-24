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
		ReferenceCounter() = default;
		ReferenceCounter(uint32 strongRefs, uint32 weakRefs) : m_strongRefs(strongRefs), m_weakRefs(weakRefs) {}

		void IncrementStrongRef()    { ++m_strongRefs; }
		void IncrementWeakRef()      { ++m_weakRefs;   }

		void DecrementStrongRef()    { --m_strongRefs; }
		void DecrementWeakRef()      { --m_weakRefs;   }

		uint32 GetStrongRefs() const { return m_strongRefs; }
		uint32 GetWeakRefs()   const { return m_weakRefs;   }

		bool HasAnyStrongRef() const { return m_strongRefs > 0; }
		bool HasAnyWeakRef()   const { return m_weakRefs   > 0; }
		bool HasAnyRef()       const { return HasAnyStrongRef() || HasAnyWeakRef(); }
	};
}