// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Graph;

import jpt.Graph.Edge;
import jpt.Graph.Node;

import jpt.Allocator;
import jpt.Constants;
import jpt.Concepts;
import jpt.TypeDefs;
import jpt.DynamicArray;

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
		using TData      = _TData;
		using Weight     = float32;
		using Edge       = GraphEdge;
		using Node       = GraphNode<TData>;

	private:
		DynamicArray<Node> m_nodes;

	public:
		// Adding
		constexpr Index AddNode(const TData& data);

		constexpr void AddEdge(Index from, Index to, Weight weight = 0.0f);
		constexpr void AddEdgeBoth(Index from, Index to, Weight weight = 0.0f);

		// Erasing

		// Accessing
		constexpr bool Count() const;
		constexpr bool IsEmpty() const;

		// Searching
		constexpr Index FindIndex(const TData& data) const;
	};

	template<typename _TData, bool kAllowDuplicates>
	constexpr Index Graph<_TData, kAllowDuplicates>::AddNode(const TData& data)
	{
		if constexpr (!kAllowDuplicates)
		{
			const Index index = FindIndex(data);
			JPT_ASSERT(index == kInvalidValue<Index>, "Can't have duplicates in this graph. Found same data at index %lu", index);
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
}