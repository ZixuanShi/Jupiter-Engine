// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ReferenceCounter;

import jpt.TypeDefs;

export namespace jpt_private
{
	class ReferenceCounter
	{
	private:
		int32 m_sharedRefs = 0;
		int32 m_weakRefs = 0;

	public:
		ReferenceCounter() = default;
		ReferenceCounter(int32 sharedRefs, int32 weakRefs)
			: m_sharedRefs(sharedRefs)
			, m_weakRefs(weakRefs) 
		{
		}

		void IncrementStrongRef() { ++m_sharedRefs; }
		void IncrementWeakRef() { ++m_weakRefs; }

		void DecrementStrongRef() { --m_sharedRefs; }
		void DecrementWeakRef() { --m_weakRefs; }

		int32 GetSharedRefs() const { return m_sharedRefs; }
		int32 GetWeakRefs() const { return m_weakRefs; }

		bool HasAnySharedRef() const { return m_sharedRefs > 0; }
		bool HasAnyWeakRef() const { return m_weakRefs > 0; }
	};
}