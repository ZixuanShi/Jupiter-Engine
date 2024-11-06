// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graph.Node;

import jpt.Graph.Edge;
import jpt.TypeDefs;

export namespace jpt
{
	template <typename TData>
	class GraphNode
	{
	private:
		Edges m_edges;	/**< Neighbors. Connected nodes */
		TData m_data;

	public:
		constexpr GraphNode() = default;
		constexpr GraphNode(const TData& data);

		constexpr void AddEdge(Index destination, Weight weight);
		constexpr void EraseEdge(Index destination);

		const TData& GetData() const;
		constexpr bool operator==(const GraphNode& other) const;
	};

	template<typename TData>
	constexpr GraphNode<TData>::GraphNode::GraphNode(const TData& data)
		: m_data(data)
	{
	}

	template<typename TData>
	constexpr void GraphNode<TData>::AddEdge(Index destination, Weight weight)
	{
		m_edges.EmplaceBack(destination, weight);
	}

	template<typename TData>
	constexpr void GraphNode<TData>::EraseEdge(Index destination)
	{
		for (size_t i = 0; i < m_edges.Count(); ++i)
		{
			if (m_edges[i].GetDestination() == destination)
			{
				m_edges.Erase(i);
				break;
			}
		}
	}

	template<typename TData>
	const TData& GraphNode<TData>::GetData() const
	{
		return m_data;
	}

	template<typename TData>
	constexpr bool GraphNode<TData>::GraphNode::operator==(const GraphNode& other) const
	{
		return m_data == other.m_data;
	}
}