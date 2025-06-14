// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graph_Edge;

import jpt.TypeDefs;
import jpt.Constants;
import jpt.DynamicArray;

export namespace jpt
{
    struct GraphEdge
    {
        Index m_destination = kInvalidValue<Index>;       /**< The destination node index */
        Precision m_weight = static_cast<Precision>(0);      /**< 0 means no weight */

        constexpr GraphEdge() = default;
        constexpr GraphEdge(Index destination, Precision weight)
            : m_destination(destination)
            , m_weight(weight)
        {
        }
    };

    using Edges = DynamicArray<GraphEdge>;
}