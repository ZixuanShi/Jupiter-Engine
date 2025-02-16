// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt_private.LinearNodeIterator;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt_private
{
	/** Linear Node Based Data container */
	template<typename TData>
	struct LinearNode
	{
		TData data;
		LinearNode* pPrevious = nullptr;
		LinearNode* pNext     = nullptr;

		constexpr LinearNode(const TData& _data) : data(_data) {}
		constexpr LinearNode(TData&& _data) : data(jpt::Move(_data)) {}

		constexpr ~LinearNode();
	};

	template<typename TData>
	constexpr LinearNode<TData>::~LinearNode()
	{
		if constexpr (!jpt::IsTriviallyDestructible<TData>)
		{
			data.~TData();
		}

		pPrevious = nullptr;
		pNext = nullptr;
	}

	/** Iterator for linear node based data, i.e. LinkedList */
	template<typename TData>
	class LinearNodeIterator
	{
	private:
		LinearNode<TData>* m_pNode;

	public:
		constexpr LinearNodeIterator() = default;
		constexpr LinearNodeIterator(LinearNode<TData>* pNode) : m_pNode(pNode) {}

		constexpr LinearNodeIterator& operator++();
		constexpr LinearNodeIterator operator++(int32);

		constexpr LinearNodeIterator& operator+=(size_t offset);
		constexpr LinearNodeIterator operator+(size_t offset);

		constexpr LinearNodeIterator& operator--();
		constexpr LinearNodeIterator operator--(int32);

		constexpr LinearNodeIterator& operator-=(size_t offset);
		constexpr LinearNodeIterator operator-(size_t offset);

		constexpr       TData* operator->()       { return &m_pNode->data; }
		constexpr const TData* operator->() const { return &m_pNode->data; }
		constexpr       TData* operator&()        { return &m_pNode->data; }
		constexpr const TData* operator&()  const { return &m_pNode->data; }
		constexpr       TData& operator*()        { return m_pNode->data; }
		constexpr const TData& operator*()  const { return m_pNode->data; }

		constexpr       LinearNode<TData>* GetNode()       { return m_pNode; }
		constexpr const LinearNode<TData>* GetNode() const { return m_pNode; }

		constexpr bool operator==(const LinearNodeIterator& other) const { return m_pNode == other.m_pNode; }
	};

	template<typename TData>
	constexpr LinearNodeIterator<TData>& LinearNodeIterator<TData>::operator++()
	{
		m_pNode = m_pNode->pNext;
		return *this;
	}

	template<typename TData>
	constexpr LinearNodeIterator<TData> LinearNodeIterator<TData>::operator++(int32)
	{
		LinearNodeIterator iterator = *this;
		m_pNode = m_pNode->pNext;
		return iterator;
	}

	template<typename TData>
	constexpr LinearNodeIterator<TData>& LinearNodeIterator<TData>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			m_pNode = m_pNode->pNext;
		}

		return *this;
	}

	template<typename TData>
	constexpr LinearNodeIterator<TData> LinearNodeIterator<TData>::operator+(size_t offset)
	{
		LinearNodeIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData>
	constexpr LinearNodeIterator<TData>& LinearNodeIterator<TData>::operator--()
	{
		m_pNode = m_pNode->pPrevious;
		return *this;
	}

	template<typename TData>
	constexpr LinearNodeIterator<TData> LinearNodeIterator<TData>::operator--(int32)
	{
		LinearNodeIterator iterator = *this;
		m_pNode = m_pNode->pPrevious;
		return iterator;
	}

	template<typename TData>
	constexpr LinearNodeIterator<TData>& LinearNodeIterator<TData>::operator-=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			m_pNode = m_pNode->pPrevious;
		}

		return *this;
	}

	template<typename TData>
	constexpr LinearNodeIterator<TData> LinearNodeIterator<TData>::operator-(size_t offset)
	{
		LinearNodeIterator iterator = *this;
		return iterator -= offset;
	}

	template<typename TData>
	class ConstLinearNodeIterator
	{
	private:
		const LinearNode<TData>* m_pNode;

	public:
		constexpr ConstLinearNodeIterator() = default;
		constexpr ConstLinearNodeIterator(const LinearNode<TData>* pNode) : m_pNode(pNode) {}

		constexpr ConstLinearNodeIterator& operator++();
		constexpr ConstLinearNodeIterator operator++(int32);

		constexpr ConstLinearNodeIterator& operator+=(size_t offset);
		constexpr ConstLinearNodeIterator operator+(size_t offset);

		constexpr ConstLinearNodeIterator& operator--();
		constexpr ConstLinearNodeIterator operator--(int32);

		constexpr ConstLinearNodeIterator& operator-=(size_t offset);
		constexpr ConstLinearNodeIterator operator-(size_t offset);

		constexpr const TData* operator->() const { return &m_pNode->data; }
		constexpr const TData* operator&()  const { return &m_pNode->data; }
		constexpr const TData& operator*()  const { return  m_pNode->data; }

		constexpr const LinearNode<TData>* GetNode() const { return m_pNode; }

		constexpr bool operator==(const ConstLinearNodeIterator& other) const { return m_pNode == other.m_pNode; }
	};

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData>& ConstLinearNodeIterator<TData>::operator++()
	{
		m_pNode = m_pNode->pNext;
		return *this;
	}

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData> ConstLinearNodeIterator<TData>::operator++(int32)
	{
		ConstLinearNodeIterator iterator = *this;
		m_pNode = m_pNode->pNext;
		return iterator;
	}

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData>& ConstLinearNodeIterator<TData>::operator+=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			m_pNode = m_pNode->pNext;
		}

		return *this;
	}

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData> ConstLinearNodeIterator<TData>::operator+(size_t offset)
	{
		ConstLinearNodeIterator iterator = *this;
		return iterator += offset;
	}

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData>& ConstLinearNodeIterator<TData>::operator--()
	{
		m_pNode = m_pNode->pPrevious;
		return *this;
	}

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData> ConstLinearNodeIterator<TData>::operator--(int32)
	{
		ConstLinearNodeIterator iterator = *this;
		m_pNode = m_pNode->pPrevious;
		return iterator;
	}

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData>& ConstLinearNodeIterator<TData>::operator-=(size_t offset)
	{
		for (size_t i = 0; i < offset; ++i)
		{
			m_pNode = m_pNode->pPrevious;
		}

		return *this;
	}

	template<typename TData>
	constexpr ConstLinearNodeIterator<TData> ConstLinearNodeIterator<TData>::operator-(size_t offset)
	{
		ConstLinearNodeIterator iterator = *this;
		return iterator -= offset;
	}
}