// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module UnitTests_Coding_BestMeetingNode;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Optional;
import jpt.Utilities;
import jpt.Heap;
import jpt.Pair;

// Given a graph. 
// N people are standing at different nodes. 
// Find the best meeting node that minimizes the total distance traveled by all people.
// The graph is directed and weighted.

constexpr Index ToIndex(char data)
{
    return static_cast<Index>(data - 'A');
}

struct Node
{
    char m_data = '0';
};

using Edge          = jpt::Pair<Index, float32>;
using Neighbors     = jpt::DynamicArray<Edge>;
using Adjacencylist = jpt::DynamicArray<Neighbors>;

// Projects/UnitTests/Assets/UnitTests_Graph_0.jpg
void BuildGraph_0(jpt::DynamicArray<Node>& nodes, Adjacencylist& adjacencyList)
{
    // Nodes
    nodes.EmplaceBack('A');  // 0
    nodes.EmplaceBack('B');  // 1
    nodes.EmplaceBack('C');  // 2
    nodes.EmplaceBack('D');  // 3
    nodes.EmplaceBack('E');  // 4
    nodes.EmplaceBack('F');  // 5
    nodes.EmplaceBack('G');  // 6
    nodes.EmplaceBack('H');  // 7
    nodes.EmplaceBack('I');  // 8
    nodes.EmplaceBack('J');  // 9

    // Edges
    adjacencyList.Resize(nodes.Count());

    adjacencyList[ToIndex('A')].EmplaceBack(ToIndex('D'), 1.0f);

    adjacencyList[ToIndex('B')].EmplaceBack(ToIndex('A'), 2.0f);
    adjacencyList[ToIndex('B')].EmplaceBack(ToIndex('E'), 2.0f);

    adjacencyList[ToIndex('C')].EmplaceBack(ToIndex('F'), 5.0f);
    adjacencyList[ToIndex('C')].EmplaceBack(ToIndex('I'), 8.0f);

    adjacencyList[ToIndex('D')].EmplaceBack(ToIndex('B'), 2.0f);
    adjacencyList[ToIndex('D')].EmplaceBack(ToIndex('E'), 4.0f);
    adjacencyList[ToIndex('D')].EmplaceBack(ToIndex('F'), 3.0f);

    adjacencyList[ToIndex('E')].EmplaceBack(ToIndex('B'), 2.0f);
    adjacencyList[ToIndex('E')].EmplaceBack(ToIndex('F'), 2.0f);
    adjacencyList[ToIndex('E')].EmplaceBack(ToIndex('J'), 3.0f);

    adjacencyList[ToIndex('F')].EmplaceBack(ToIndex('C'), 5.0f);
    adjacencyList[ToIndex('F')].EmplaceBack(ToIndex('E'), 2.0f);
    adjacencyList[ToIndex('F')].EmplaceBack(ToIndex('G'), 3.0f);

    adjacencyList[ToIndex('G')].EmplaceBack(ToIndex('A'), 15.0f);
    adjacencyList[ToIndex('G')].EmplaceBack(ToIndex('D'), 2.0f);

    adjacencyList[ToIndex('I')].EmplaceBack(ToIndex('C'), 8.0f);
    adjacencyList[ToIndex('I')].EmplaceBack(ToIndex('F'), 10.0f);
}

float32 GetDistanceToNode(const Adjacencylist& adjacencyList, const Node& from, const Node& to)
{
    struct SearchData
    {
        float32 distance = jpt::Constants<float32>::kMax;
        Index previous = kInvalidIndex;
        bool visited = false;
    };

    jpt::DynamicArray<SearchData> searchData(adjacencyList.Count());
    auto comparator = [&searchData](Index lhs, Index rhs) -> bool
        {
            return searchData[lhs].distance < searchData[rhs].distance;
        };

    using OpenSet = jpt::PriorityQueue<Index, decltype(comparator)>;
    OpenSet openSet(comparator);

    searchData[ToIndex(from.m_data)].distance = 0.0f;
    openSet.Emplace(ToIndex(from.m_data));

    while (!openSet.IsEmpty())
    {
        const Index current = openSet.Top();
        openSet.Pop();

        // Found the destination. Sum to the distance
        if (current == ToIndex(to.m_data))
        {
            return searchData[current].distance;
        }

        searchData[current].visited = true;

        for (const Edge& edge : adjacencyList[current])
        {
            if (!searchData[edge.first].visited)
            {
                const float32 newDistance = searchData[current].distance + edge.second;
                if (newDistance < searchData[edge.first].distance)
                {
                    searchData[edge.first].distance = newDistance;
                    searchData[edge.first].previous = current;
                    openSet.Emplace(edge.first);
                }
            }
        }
    }

    return jpt::Constants<float32>::kMax;
}

Index FindBestMeetingNode(const jpt::DynamicArray<Node>& nodes, const Adjacencylist& adjacencyList, const jpt::DynamicArray<Node>& people)
{
    Index bestNode = kInvalidIndex;
    float32 shortestDistance = jpt::Constants<float32>::kMax;

    // For each node. Calculate the total distance traveled by all people.
    for (const Node& node : nodes)
    {
        float32 totalDistance = 0.0f;

        // For each person . Calculate the distance traveled to the node.
        for (const Node& person : people)
        {
            const float32 distance = GetDistanceToNode(adjacencyList, person, node);

            // No path found. Skip this node
            if (distance == jpt::Constants<float32>::kMax)
            {
                totalDistance = jpt::Constants<float32>::kMax;
                break;
            }
            else
            {
                totalDistance += distance;
            }
        }

        if (totalDistance < shortestDistance)
        {
            shortestDistance = totalDistance;
            bestNode = ToIndex(node.m_data);
        }
    }

    return bestNode;
}

export bool UnitTests_Coding_BestMeetingNode()
{
    jpt::DynamicArray<Node> nodes;
    jpt::DynamicArray<Node> people;
    Adjacencylist adjacencyList;

    BuildGraph_0(nodes, adjacencyList);

    people = { { 'A' }, { 'B' }, { 'C' } };
    JPT_ENSURE(FindBestMeetingNode(nodes, adjacencyList, people) == ToIndex('B'));

    people = { { 'A' }, { 'G' }, { 'D' }, { 'I' } };
    JPT_ENSURE(FindBestMeetingNode(nodes, adjacencyList, people) == ToIndex('D'));

    return true;
}