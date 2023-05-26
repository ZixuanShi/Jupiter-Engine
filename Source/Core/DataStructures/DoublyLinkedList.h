#pragma once

namespace jpt
{
	template<class _ValueType>
	class list
	{
	private:
		// The node this linked list is using to store data and the previous/next node
		struct Node
		{
			_ValueType data;
			Node* pPrevious = nullptr;
			Node* pNext     = nullptr;

			Node(_ValueType inData) : data(inData) {}
			~Node()
			{
				if constexpr (!std::is_trivially_destructible_v<_ValueType>)
				{
					data.~_ValueType();
				}

				pPrevious = nullptr;
				pNext = nullptr;
			}
		};

		template<class ListType>
		class list_iterator
		{
			template<typename unordered_map_type>
			friend class unordered_map_iterator;

		private:
			Node* m_pNode = nullptr;

		public:
			list_iterator(Node* pNode) : m_pNode(pNode) {}

			_ValueType* operator->() { return &m_pNode->data; }
			_ValueType* operator&() { return &m_pNode->data; }
			_ValueType& operator*() { return m_pNode->data; }
			bool operator==(const list_iterator& other) const { return m_pNode == other.m_pNode; }
			bool operator!=(const list_iterator& other) const { return m_pNode != other.m_pNode; }
			
			list_iterator& operator++()
			{
				m_pNode = m_pNode->pNext;
				return *this;
			}

			list_iterator operator++(int32)
			{
				list_iterator iterator = *this;
				++(*this);
				return iterator;
			}

			list_iterator& operator--()
			{
				m_pNode = m_pNode->pPrevious;
				return *this;
			}

			list_iterator operator--(int32)
			{
				list_iterator iterator = *this;
				--(*this);
				return iterator;
			}
		};

	public:
		using ValueType = _ValueType;
		using iterator  = list_iterator<list<ValueType>>;
		
	private:
		Node* m_pHead = nullptr;	// Start of this linkedlist
		Node* m_pTail = nullptr;	// End of this linkedlist
		size_t m_size = 0;

	public:
		list() = default;
		list(const std::initializer_list<ValueType>& initializerList);
		list(const list& other);
		list(list&& other) noexcept;
		list& operator=(const list& other);
		list& operator=(list&& other) noexcept;
		~list();

		// Element access
		ValueType& front() { return m_pHead->data; }
		const ValueType& front() const { return m_pHead->data; }
		ValueType& back() { return m_pTail->data; }
		const ValueType& back() const { return m_pTail->data; }

		// Capacity
		bool empty() const { return m_size == 0; }
		size_t size() const { return m_size; }

		// Iterators
		iterator begin() noexcept { return iterator(m_pHead); }
		const iterator begin() const noexcept { return iterator(m_pHead); }
		const iterator cbegin() const noexcept { return iterator(m_pHead); }
		iterator end() noexcept;
		const iterator end() const noexcept;
		const iterator cend() const noexcept;

		// Modifiers
		void clear();
		void push_back(const ValueType& value);
		void push_back(ValueType&& value);
		template<class...Args> void emplace_back(Args&&... args);
		void push_front(const ValueType& value);
		void push_front(ValueType&& value);
		template<class...Args> void emplace_front(Args&&... args);
		void insert(size_t index, const ValueType& value);
		void insert(size_t index, ValueType&& value);
		void pop_front();
		void pop_back();
		void erase(size_t index);

	private:
		// Adds a recent created node to this list.
		// - pNewNode: The new node to add. Should be heap allocated already
		// - index: The index to add the node. 0 for head. m_size for tail
		void AddNode(Node* pNewNode, size_t index);

		// Remove a node from this list.
		// - index: The index to remove the node. 0 for head. m_size for tail
		void RemoveNode(size_t index);

		void CopyList(const list& other);
		void TakeList(list&& other);
	};

	template<class _ValueType>
	inline list<_ValueType>::list(const std::initializer_list<ValueType>& initializerList)
	{
		for (auto itr = initializerList.begin(); itr != initializerList.end(); ++itr)
		{
			emplace_back(*itr);
		}
	}

	template<class _ValueType>
	inline list<_ValueType>::list(const list<ValueType>& other)
	{
		CopyList(other);
	}

	template<class _ValueType>
	inline list<_ValueType>::list(list<ValueType>&& other) noexcept
	{
		TakeList(jpt::move(other));
	}

	template<class _ValueType>
	inline list<_ValueType>& list<_ValueType>::operator=(const list<ValueType>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		CopyList(other);
		return *this;
	}

	template<class _ValueType>
	inline list<_ValueType>& list<_ValueType>::operator=(list<ValueType>&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		clear();
		TakeList(jpt::move(other));
		return *this;
	}

	template<class _ValueType>
	inline list<_ValueType>::~list()
	{
		clear();
	}

	template<class _ValueType>
	inline list<_ValueType>::iterator list<_ValueType>::end() noexcept
	{
		if (empty())
		{
			return iterator(m_pTail);
		}

		return iterator(m_pTail->pNext);
	}

	template<class _ValueType>
	inline const list<_ValueType>::iterator list<_ValueType>::end() const noexcept
	{
		if (empty())
		{
			return iterator(m_pTail);
		}

		return iterator(m_pTail->pNext);
	}

	template<class _ValueType>
	inline const list<_ValueType>::iterator list<_ValueType>::cend() const noexcept
	{
		if (empty())
		{
			return iterator(m_pTail);
		}

		return iterator(m_pTail->pNext);
	}

	template<class _ValueType>
	inline void list<_ValueType>::clear()
	{
		Node* pCurrent = m_pHead;
		while (pCurrent)
		{
			Node* pTemp = pCurrent;
			pCurrent = pCurrent->pNext;
			JPT_DELETE(pTemp);
		}

		m_pHead = nullptr;
		m_pTail = nullptr;
		m_size = 0;
	}

	template<class _ValueType>
	inline void list<_ValueType>::push_back(const ValueType& value)
	{
		Node* pNewNode = new Node(value);
		AddNode(pNewNode, m_size);
	}

	template<class _ValueType>
	inline void list<_ValueType>::push_back(ValueType&& value)
	{
		Node* pNewNode = new Node(jpt::move(value));
		AddNode(pNewNode, m_size);
	}
	
	template<class _ValueType>
	template<class ...Args>
	inline void list<_ValueType>::emplace_back(Args&& ...args)
	{
		Node* pNewNode = new Node(std::forward<Args>(args)...);
		AddNode(pNewNode, m_size);
	}

	template<class _ValueType>
	inline void list<_ValueType>::push_front(const ValueType& value)
	{
		Node* pNewNode = new Node(value);
		AddNode(pNewNode, 0);
	}

	template<class _ValueType>
	inline void list<_ValueType>::push_front(ValueType&& value)
	{
		Node* pNewNode = new Node(jpt::move(value));
		AddNode(pNewNode, 0);
	}

	template<class _ValueType>
	template<class ...Args>
	inline void list<_ValueType>::emplace_front(Args&& ...args)
	{
		Node* pNewNode = new Node(std::forward<Args>(args)...);
		AddNode(pNewNode, 0);
	}

	template<class _ValueType>
	inline void list<_ValueType>::insert(size_t index, const ValueType& value)
	{
		Node* pNewNode = new Node(value);
		AddNode(pNewNode, index);
	}

	template<class _ValueType>
	inline void list<_ValueType>::insert(size_t index, ValueType&& value)
	{
		Node* pNewNode = new Node(jpt::move(value));
		AddNode(pNewNode, index);
	}

	template<class _ValueType>
	inline void list<_ValueType>::pop_front()
	{
		RemoveNode(0);
	}

	template<class _ValueType>
	inline void list<_ValueType>::pop_back()
	{
		RemoveNode(m_size);
	}

	template<class _ValueType>
	inline void list<_ValueType>::erase(size_t index)
	{
		RemoveNode(index);
	}

	template<class _ValueType>
	inline void list<_ValueType>::AddNode(Node* pNewNode, size_t index)
	{
		JPT_ASSERT(pNewNode, "Trying to add an invalid node to this list");
		JPT_ASSERT(index <= m_size, "Trying to add a node at an invalid index");

		// Empty list
		if (empty())
		{
			m_pHead = pNewNode;
			m_pTail = pNewNode;
		}
		// Add in front
		else if (index == 0)
		{
			pNewNode->pNext = m_pHead;
			m_pHead->pPrevious = pNewNode;
			m_pHead = pNewNode;
		}
		// Add to Back
		else if (index == m_size)
		{
			pNewNode->pPrevious = m_pTail;
			m_pTail->pNext = pNewNode;
			m_pTail = pNewNode;
		}
		// Insert in the middle. Slower than the other three
		else
		{
			Node* pPrevious = nullptr;
			Node* pNext = nullptr;

			// If index is closer to the head, start from head
			if (index < (m_size / 2))
			{
				pPrevious = m_pHead;
				pNext = m_pHead->pNext;

				for (size_t i = 0; i < index; ++i)
				{
					pPrevious = pPrevious->pNext;
					pNext = pNext->pNext;
				}
			}
			// If index is closer to the tail, start from tail instead
			else
			{
				pPrevious = m_pTail->pPrevious;
				pNext = m_pTail;

				for (size_t i = m_size - 1; i > index; --i)
				{
					pPrevious = pPrevious->pPrevious;
					pNext = pNext->pPrevious;
				}
			}

			pPrevious->pNext = pNewNode;
			pNewNode->pPrevious = pPrevious;

			pNext->pPrevious = pNewNode;
			pNewNode->pNext = pNext;
		}

		++m_size;
	}

	template<class _ValueType>
	inline void list<_ValueType>::RemoveNode(size_t index)
	{
		JPT_ASSERT(index <= m_size, "Trying to remove a node at an invalid index");

		Node* pNodeToRemove = nullptr;

		// If index is closer to the head, start from head
		if (index < (m_size / 2))
		{
			pNodeToRemove = m_pHead;
			for (size_t i = 0; i < index; ++i)
			{
				pNodeToRemove = pNodeToRemove->pNext;
			}
		}
		// If index is closer to the tail, start from tail instead
		else
		{
			pNodeToRemove = m_pTail;
			for (size_t i = m_size - 1; i > index; --i)
			{
				pNodeToRemove = pNodeToRemove->pPrevious;
			}
		}

		Node* pPrevious = pNodeToRemove->pPrevious;
		Node* pNext = pNodeToRemove->pNext;

		JPT_DELETE(pNodeToRemove);

		if (pPrevious)
		{
			pPrevious->pNext = pNext;
		}
		else
		{
			m_pHead = pNext;
		}

		if (pNext)
		{
			pNext->pPrevious = pPrevious;
		}
		else
		{
			m_pTail = pPrevious;
		}

		--m_size;
	}

	template<class _ValueType>
	inline void list<_ValueType>::CopyList(const list& other)
	{
		for (auto itr = other.begin(); itr != other.end(); ++itr)
		{
			emplace_back(*itr);
		}
	}

	template<class _ValueType>
	inline void list<_ValueType>::TakeList(list&& other)
	{
		m_pHead = other.m_pHead;
		m_pTail = other.m_pTail;
		m_size = other.m_size;

		other.m_pHead = nullptr;
		other.m_pTail = nullptr;
		other.m_size = 0;
	}
}