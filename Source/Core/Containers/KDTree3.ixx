// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.KDTree3;

import jpt.Concepts;
import jpt.Sort;
import jpt.DynamicArray;
import jpt.TVector3;
import jpt_private.BinaryTreeIterator;

namespace jpt
{
	/** 3D K-D Tree implementation */
	export template<Floating T>
	class KDTree3
	{
	private:
		using Node = jpt_private::BinaryTreeNode<TVector3<T>>;

	private:
		Node* m_pRoot = nullptr;
		size_t m_count = 0;

	public:
		constexpr KDTree3() = default;
		constexpr KDTree3(const DynamicArray<TVector3<T>>& points);
		constexpr ~KDTree3();

		// Modifiers
		constexpr void Add(const TVector3<T>& point);
		constexpr void Erase(const TVector3<T>& point);
		constexpr void Clear();

		// Capacity
		constexpr size_t Count() const;
		constexpr bool IsEmpty() const;

		// Building
		constexpr void Build(const DynamicArray<TVector3<T>>& points);

		// Searching
		constexpr bool Has(const TVector3<T>& point) const;
		constexpr DynamicArray<TVector3<T>> FindNearest(const TVector3<T>& point, T threshold) const;

	private:
		// Building
		constexpr Node* InternalBuild(const DynamicArray<TVector3<T>>& points, size_t begin, size_t end, size_t depth);

		// Searching
		void RecurFindNearest(Node* pNode, const TVector3<T>& point, T threshold, size_t depth, DynamicArray<TVector3<T>>& result) const;

		// Traversal
		template<typename TFunc>
		void RecurPostOrderWalkNode(Node* pNode, TFunc&& func);
	};

	template<Floating T>
	constexpr KDTree3<T>::KDTree3(const DynamicArray<TVector3<T>>& points)
	{
		Build(points);
	}

	template<Floating T>
	constexpr KDTree3<T>::~KDTree3()
	{
		Clear();
	}

	template<Floating T>
	constexpr void KDTree3<T>::Add(const TVector3<T>& point)
	{
		Node* pNode = m_pRoot;
		size_t depth = 0;

		while (true)
		{
			size_t axis = depth % 3;

			if (point[axis] < pNode->data[axis])
			{
				if (pNode->pLeft == nullptr)
				{
					pNode->pLeft = new Node();
					pNode->pLeft->data = point;
					break;
				}
				else
				{
					pNode = pNode->pLeft;
				}
			}
			else
			{
				if (pNode->pRight == nullptr)
				{
					pNode->pRight = new Node();
					pNode->pRight->data = point;
					break;
				}
				else
				{
					pNode = pNode->pRight;
				}
			}

			++depth;
		}

		++m_count;
	}

	template<Floating T>
	constexpr void KDTree3<T>::Erase(const TVector3<T>& point)
	{
		Node* pNode = m_pRoot;
		Node* pParent = nullptr;
		size_t depth = 0;

		// Find the node to erase
		while (pNode != nullptr)
		{
			size_t axis = depth % 3;

			if (point[axis] == pNode->data[axis])
			{
				break;
			}

			pParent = pNode;

			if (point[axis] < pNode->data[axis])
			{
				pNode = pNode->pLeft;
			}
			else
			{
				pNode = pNode->pRight;
			}

			++depth;
		}

		if (pNode == nullptr)
		{
			return;
		}

		// If the node is a leaf node, just delete it
		if (pNode->pLeft == nullptr && pNode->pRight == nullptr)
		{
			if (pParent == nullptr)
			{
				delete pNode;
				m_pRoot = nullptr;
			}
			else if (pParent->pLeft == pNode)
			{
				delete pNode;
				pParent->pLeft = nullptr;
			}
			else
			{
				delete pNode;
				pParent->pRight = nullptr;
			}
		}
		// If the node has only one child, replace the node with the child
		else if (pNode->pLeft != nullptr && pNode->pRight == nullptr)
		{
			if (pParent == nullptr)
			{
				m_pRoot = pNode->pLeft;
				delete pNode;
			}
			else if (pParent->pLeft == pNode)
			{
				pParent->pLeft = pNode->pLeft;
				delete pNode;
			}
			else
			{
				pParent->pRight = pNode->pLeft;
				delete pNode;
			}
		}
		else if (pNode->pLeft == nullptr && pNode->pRight != nullptr)
		{
			if (pParent == nullptr)
			{
				m_pRoot = pNode->pRight;
				delete pNode;
			}
			else if (pParent->pLeft == pNode)
			{
				pParent->pLeft = pNode->pRight;
				delete pNode;
			}
			else
			{
				pParent->pRight = pNode->pRight;
				delete pNode;
			}
		}
		// If the node has two children, replace the node with the minimum node in the right subtree
		else
		{
			Node* pMinNode = pNode->pRight;
			Node* pMinParent = pNode;

			while (pMinNode->pLeft != nullptr)
			{
				pMinParent = pMinNode;
				pMinNode = pMinNode->pLeft;
			}

			pNode->data = pMinNode->data;

			if (pMinParent->pLeft == pMinNode)
			{
				pMinParent->pLeft = pMinNode->pRight;
			}
			else
			{
				pMinParent->pRight = pMinNode->pRight;
			}

			delete pMinNode;
		}

		--m_count;
	}

	template<Floating T>
	constexpr void KDTree3<T>::Clear()
	{
		RecurPostOrderWalkNode(m_pRoot, [](Node* pNode) 
			{ 
				delete pNode; 
				pNode = nullptr;
			});

		m_pRoot = nullptr;
		m_count = 0;
	}

	template<Floating T>
	constexpr size_t KDTree3<T>::Count() const
	{
		return m_count;
	}

	template<Floating T>
	constexpr bool KDTree3<T>::IsEmpty() const
	{
		return m_count == 0;
	}

	template<Floating T>
	constexpr void KDTree3<T>::Build(const DynamicArray<TVector3<T>>& points)
	{
		Clear();
		m_pRoot = InternalBuild(points, 0, points.Count(), 0);
		m_count = points.Count();
	}

	template<Floating T>
	constexpr bool KDTree3<T>::Has(const TVector3<T>& point) const
	{
		Node* pNode = m_pRoot;
		size_t depth = 0;

		while (pNode != nullptr)
		{
			size_t axis = depth % 3;

			if (point[axis] == pNode->data[axis])
			{
				return true;
			}

			if (point[axis] < pNode->data[axis])
			{
				pNode = pNode->pLeft;
			}
			else
			{
				pNode = pNode->pRight;
			}

			++depth;
		}

		return false;
	}

	template<Floating T>
	constexpr DynamicArray<TVector3<T>> KDTree3<T>::FindNearest(const TVector3<T>& point, T threshold) const
	{
		DynamicArray<TVector3<T>> result;
		RecurFindNearest(m_pRoot, point, threshold, 0, result);
		return result;
	}

	template<Floating T>
	constexpr typename KDTree3<T>::Node* KDTree3<T>::InternalBuild(const DynamicArray<TVector3<T>>& points, size_t begin, size_t end, size_t depth)
	{
		if (begin >= end)
		{
			return nullptr;
		}

		size_t axis = depth % 3;
		size_t mid = begin + (end - begin) / 2;

		// Sort the points based on the axis so that we can find the median
		DynamicArray<TVector3<T>> sortedpoints = points;
		jpt::Sort(sortedpoints, [axis](const TVector3<T>& a, const TVector3<T>& b)
			{
				return a[axis] < b[axis];
			});

		// Build the node and recur to the left and right child
		KDTree3<T>::Node* pNode = new KDTree3<T>::Node();
		pNode->data = sortedpoints[mid];
		pNode->pLeft = InternalBuild(sortedpoints, begin, mid, depth + 1);
		pNode->pRight = InternalBuild(sortedpoints, mid + 1, end, depth + 1);

		return pNode;
	}

	template<Floating T>
	void KDTree3<T>::RecurFindNearest(Node* pNode, const TVector3<T>& point, T threshold, size_t depth, DynamicArray<TVector3<T>>& result) const
	{
		if (pNode == nullptr)
		{
			return;
		}

		size_t axis = depth % 3;
		size_t count = 0;

		// Check if the current node is closer than threshold, if so add it to the result
		if (Vec3f::Distance(pNode->data, point) < threshold)
		{
			result.EmplaceBack(pNode->data);
			++count;
		}

		// Recur to the left or right child based on the axis
		if (point[axis] - threshold < pNode->data[axis])
		{
			RecurFindNearest(pNode->pLeft, point, threshold, depth + 1, result);
		}
		if (point[axis] + threshold >= pNode->data[axis])
		{
			RecurFindNearest(pNode->pRight, point, threshold, depth + 1, result);
		}
	}

	template<Floating T>
	template<typename TFunc>
	void KDTree3<T>::RecurPostOrderWalkNode(Node* pNode, TFunc&& func)
	{
		if (pNode)
		{
			RecurPostOrderWalkNode(pNode->pLeft, func);
			RecurPostOrderWalkNode(pNode->pRight, func);
			func(pNode);
		}
	}
}

export using KDTree3f = jpt::KDTree3<float>;