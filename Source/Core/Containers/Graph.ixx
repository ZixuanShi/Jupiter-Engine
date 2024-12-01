// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.Graph;

import jpt.Graph.Edge;
import jpt.Graph.Node;

import jpt.Allocator;
import jpt.Constants;
import jpt.Concepts;
import jpt.TypeDefs;
import jpt.Function;
import jpt.Utilities;
import jpt.Optional;

import jpt.DynamicArray;
import jpt.Queue;
import jpt.Heap;

export namespace jpt
{
	/** Graph implementation
		@tparam _TData  			The data type of the nodes
		@tparam kAllowDuplicates	True if the graph can have duplicated data in nodes

		Nodes:
		- Can have duplicated data. But must use handles to add edges, accessing, and removing nodes, otherwise will have ambiguous results as it doesn't know which node to use
		- If guaranteed no duplicated data, can use data in provided functions directly

		Index:
		- Is an index into the node array. Retrieved when Add() a node
		- Can be used to add edges, accessing, and removing nodes

		Edges:
		- Directed and unique. If an edge is added from A to B, it is not implied that there is an edge from B to A
		- Can be weighted. 0.0f means no weight */
	template<typename _TData, bool kAllowDuplicates = true>
	class Graph
	{
	public:
		using TData  = _TData;
		using Weight = float32;
		using Edge   = GraphEdge;
		using Node   = GraphNode<TData>;
		using Path   = DynamicArray<Index>;
		using WalkerFunc = Function<void(const TData&)>;

	private:
		DynamicArray<Node> m_nodes;

	public:		
		constexpr Index AddNode(const TData& data);

		constexpr void AddEdge(Index from, Index to, Weight weight = 0.0f);
		constexpr void AddEdgeBoth(Index from, Index to, Weight weight = 0.0f);

		// Erasing
		constexpr void EraseNode(Index index);

		// Accessing
		constexpr bool Count() const;
		constexpr bool IsEmpty() const;
		constexpr TData& operator[](Index index);
		constexpr const TData& operator[](Index index) const;

		// Searching
		constexpr DynamicArray<Index> FindIndices(const TData& data) const;
		constexpr Index FindIndex(const TData& data) const;
		
		// Graph algorithms
		constexpr void DFS(Index start, const WalkerFunc& walker) const;
		constexpr void BFS(Index start, const WalkerFunc& walker) const;
		constexpr Optional<Path> Dijkstra(Index start, Index end) const;

	private:
		constexpr void Recur_DFS(Index index, DynamicArray<bool>& visited, const WalkerFunc& walker) const;
	};

	template<typename TData, bool kAllowDuplicates>
	constexpr Index Graph<TData, kAllowDuplicates>::AddNode(const TData& data)
	{
		if constexpr (!kAllowDuplicates)
		{
			for (size_t i = 0; i < m_nodes.Count(); ++i)
			{
				if (m_nodes[i].GetData() == data)
				{
					JPT_ASSERT(false, "Duplicate data not allowed");
					return i;
				}
			}
		}

		m_nodes.EmplaceBack(data);
		return m_nodes.Count() - 1;
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr void Graph<_TData, kAllowDuplicates>::AddEdge(Index from, Index to, Weight weight)
	{
		JPT_ASSERT(from != to, "Can't have self edges");
		JPT_ASSERT(from < m_nodes.Count(), "Invalid from node");
		JPT_ASSERT(to < m_nodes.Count(), "Invalid to node");

		Node& fromNode = m_nodes[from];
		fromNode.AddEdge(to, weight);
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr void Graph<_TData, kAllowDuplicates>::AddEdgeBoth(Index from, Index to, Weight weight)
	{
		AddEdge(from, to, weight);
		AddEdge(to, from, weight);
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr void Graph<_TData, kAllowDuplicates>::EraseNode(Index index)
	{
		JPT_ASSERT(index < m_nodes.Count(), "Invalid node index");

		for (Node& node : m_nodes)
		{
			if (node != m_nodes[index])
			{
				node.OnEraseEdge(index);
			}
		}

		m_nodes.Erase(index);
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr bool Graph<_TData, kAllowDuplicates>::Count() const
	{
		return m_nodes.Count();
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr bool Graph<_TData, kAllowDuplicates>::IsEmpty() const
	{
		return m_nodes.IsEmpty();
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr Graph<_TData, kAllowDuplicates>::TData& Graph<_TData, kAllowDuplicates>::operator[](Index index)
	{
		return m_nodes[index].GetData();
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr const Graph<_TData, kAllowDuplicates>::TData& Graph<_TData, kAllowDuplicates>::operator[](Index index) const
	{
		return m_nodes[index].GetData();
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr DynamicArray<Index> Graph<_TData, kAllowDuplicates>::FindIndices(const TData& data) const
	{
		DynamicArray<Index> indices;

		for (size_t i = 0; i < m_nodes.Count(); ++i)
		{
			if (m_nodes[i].GetData() == data)
			{
				indices.EmplaceBack(i);
			}
		}

		if constexpr (!kAllowDuplicates)
		{
			JPT_ASSERT(indices.Count() <= 1, "Duplicate data not allowed");
		}

		return indices;
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr Index Graph<_TData, kAllowDuplicates>::FindIndex(const TData& data) const
	{
		for (size_t i = 0; i < m_nodes.Count(); ++i)
		{
			if (m_nodes[i].GetData() == data)
			{
				return i;
			}
		}

		return kInvalidValue<Index>;
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr void Graph<_TData, kAllowDuplicates>::DFS(Index start, const WalkerFunc& walker) const
	{
		DynamicArray<bool> visited(m_nodes.Count(), false);
		Recur_DFS(start, visited, walker);
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr void Graph<_TData, kAllowDuplicates>::Recur_DFS(Index index, DynamicArray<bool>& visited, const WalkerFunc& walker) const
	{
		visited[index] = true;
		walker(m_nodes[index].GetData());

		for (const Edge& edge : m_nodes[index].GetEdges())
		{
			if (!visited[edge.m_destination])
			{
				Recur_DFS(edge.m_destination, visited, walker);
			}
		}
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr void Graph<_TData, kAllowDuplicates>::BFS(Index start, const WalkerFunc& walker) const
	{
		DynamicArray<bool> visited(m_nodes.Count(), false);
		Queue<Index> queue;

		queue.Enqueue(start);
		visited[start] = true;

		while (!queue.IsEmpty())
		{
			const Index index = queue.Front();
			queue.Dequeue();
			walker(m_nodes[index].GetData());

			for (const Edge& edge : m_nodes[index].GetEdges())
			{
				if (!visited[edge.m_destination])
				{
					queue.Enqueue(edge.m_destination);
					visited[edge.m_destination] = true;
				}
			}
		}
	}

	template<typename _TData, bool kAllowDuplicates>
	constexpr Optional<typename Graph<_TData, kAllowDuplicates>::Path> Graph<_TData, kAllowDuplicates>::Dijkstra(Index start, Index end) const
	{
		struct SearchData
		{
			Weight distance = Constants<Weight>::kMax;
			Index previous = kInvalidValue<Index>;
			bool visited = false;
		};

		DynamicArray<SearchData> searchData(m_nodes.Count());
		auto comparator = [&searchData](Index lhs, Index rhs) -> bool
			{
				return searchData[lhs].distance < searchData[rhs].distance;
			};
		
		using OpenSet = PriorityQueue<Index, decltype(comparator)>;
		OpenSet openSet(comparator);

		searchData[start].distance = 0.0f;
		openSet.Emplace(start);

		while (!openSet.IsEmpty())
		{
			const Index current = openSet.Top();
			openSet.Pop();

			// Found the destination. Reconstruct the path
			if (current == end)
			{
				Path path;
				Index i = end;

				while (i != kInvalidValue<Index>)
				{
					path.EmplaceBack(i);
					i = searchData[i].previous;
				}

				path.Reverse();
				return path;
			}

			searchData[current].visited = true;

			for (const Edge& edge : m_nodes[current].GetEdges())
			{
				if (!searchData[edge.m_destination].visited)
				{
					const Weight newDistance = searchData[current].distance + edge.m_weight;

					if (newDistance < searchData[edge.m_destination].distance)
					{
						searchData[edge.m_destination].distance = newDistance;
						searchData[edge.m_destination].previous = current;
						openSet.Emplace(edge.m_destination);
					}
				}
			}
		}

		return {};
	}
}