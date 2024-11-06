// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graph.Edge;

import jpt.TypeDefs;
import jpt.Constants;
import jpt.DynamicArray;

export namespace jpt
{
	class GraphEdge
	{
		Index m_destination = kInvalidValue<Index>;	/**< The destination node index */
		Weight m_weight = static_cast<Weight>(0);		/**< 0 means no weight */

	public:
		constexpr GraphEdge() = default;
		constexpr GraphEdge(Index destination, Weight weight);

		constexpr Index GetDestination() const;
		constexpr Weight GetWeight() const;
	};

	using Edges = DynamicArray<GraphEdge>;

	constexpr GraphEdge::GraphEdge(Index destination, Weight weight)
		: m_destination(destination)
		, m_weight(weight)
	{
	}

	constexpr Index GraphEdge::GetDestination() const
	{
		return m_destination;
	}

	constexpr Weight GraphEdge::GetWeight() const
	{
		return m_weight;
	}
}