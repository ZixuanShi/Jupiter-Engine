// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.ReferenceCounter;

import jpt.TypeDefs;

export namespace jpt_private
{
	class ReferenceCounter
	{
	private:
		int32 m_strongRefs = 0;
		int32 m_weakRefs = 0;

	public:
		ReferenceCounter() = default;
		ReferenceCounter(int32 strongRef) : m_strongRefs(strongRef) {}

		void IncrementStrongRef() { ++m_strongRefs; }
		void IncrementWeakRef() { ++m_weakRefs; }

		void DecrementStrongRef() { --m_strongRefs; }
		void DecrementWeakRef() { --m_weakRefs; }

		int32 GetSharedRefs() const { return m_strongRefs; }
		int32 GetWeakRefs() const { return m_weakRefs; }

		bool HasAnySharedRef() const { return m_strongRefs > 0; }
		bool HasAnyWeakRef() const { return m_weakRefs > 0; }
		bool HasAnyRef() const { return HasAnySharedRef() || HasAnyWeakRef(); }
	};
}