// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graph.Node;

import jpt.Graph.Edge;
import jpt.TypeDefs;

export namespace jpt
{
    template<typename TData>
    class GraphNode
    {
        using Edge = GraphEdge;

    private:
        Edges m_edges;    /**< Neighbors. Connected nodes */
        TData m_data;

    public:
        constexpr GraphNode() = default;
        constexpr GraphNode(const TData& data);

        constexpr void AddEdge(Index destination, Weight weight);
        constexpr void OnEraseEdge(Index destination);

        TData& GetData();
        const TData& GetData() const;

        Edges& GetEdges();
        const Edges& GetEdges() const;

    };

    template<typename TData>
    constexpr bool operator==(const GraphNode<TData>& lhs, const GraphNode<TData>& rhs) noexcept
    {
        return lhs.GetData() == rhs.GetData();
    }

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
    constexpr void GraphNode<TData>::OnEraseEdge(Index destination)
    {
        for (int64 i = static_cast<int64>(m_edges.Count()) - 1; i >= 0; --i)
        {
            // 1. Erase edges that point to this destination
            if (m_edges[i].m_destination == destination)
            {
                m_edges.Erase(i);
            }
            // 2. Update indices after the erased index
            else if (m_edges[i].m_destination > destination)
            {
                --m_edges[i].m_destination;
            }
        }
    }

    template<typename TData>
    TData& GraphNode<TData>::GetData()
    {
        return m_data;
    }

    template<typename TData>
    const TData& GraphNode<TData>::GetData() const
    {
        return m_data;
    }

    template<typename TData>
    Edges& GraphNode<TData>::GetEdges()
    {
        return m_edges;
    }

    template<typename TData>
    const Edges& GraphNode<TData>::GetEdges() const
    {
        return m_edges;
    }
}