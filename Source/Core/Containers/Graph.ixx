// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Graph;

import jpt.Allocator;
import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt
{
	/** Graph implementation

		Nodes:
		- Can have duplicated data

		NodeHandle:
		- Is an index into the node array. Retrieved when Add() a node
		- Can be used to add edges, accessing, and removing nodes

		Edges:
		- Directed and unique. If an edge is added from A to B, it is not implied that there is an edge from B to A
		- Can be weighted. 0.0f means no weight */
	template<typename _TData, typename _TAllocator = Allocator<_TData>>
	class Graph
	{
	public:
		using TData      = _TData;
		using Weight     = float32;
		using NodeHandle = size_t;	// Index into the node array

	private:
		struct Node;

		struct Edge
		{
			Node* pFrom   = nullptr;
			Node* pTo     = nullptr;

			Weight weight = 0.0f;	// 0.0f means no weight
		};

		struct Node
		{
			TData data;
		};

	private:


	public:
		constexpr NodeHandle AddNode(const TData& data);

		constexpr void AddEdge(NodeHandle from, NodeHandle to, Weight weight = 0.0f);

		/** @warning This version of AddEdge() can't have duplicated node data */
		constexpr void AddEdge(const TData& from, const TData& to, Weight weight = 0.0f);
	};

	template<typename _TData, typename _TAllocator>
	constexpr Graph<_TData, _TAllocator>::NodeHandle Graph<_TData, _TAllocator>::AddNode(const TData&)
	{
		return NodeHandle();
	}

	template<typename _TData, typename _TAllocator>
	constexpr void Graph<_TData, _TAllocator>::AddEdge(NodeHandle, NodeHandle, Weight)
	{
	}

	template<typename _TData, typename _TAllocator>
	constexpr void Graph<_TData, _TAllocator>::AddEdge(const TData&, const TData&, Weight)
	{
	}
}