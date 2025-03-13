module;

#include "Debugging/Logger.h"

export module UnitTests_Coding_MaximumVisitableCities;

import jpt.Utilities;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.Pair;
import jpt.HashSet;
import jpt.Math;


export bool UnitTests_Coding_MaximumVisitableCities()
{

    return true;
}

//std::vector<std::vector<int>> g_adjacencyList;
//
//void Recur_TraverseDFS(int current, std::unordered_set<int>& visited, int& outMaxCities, bool usedTicket)
//{
//    // Mark current visited
//    visited.emplace(current);
//    outMaxCities = std::max(outMaxCities, static_cast<int>(visited.size()));
//
//    // Try each neighbor
//    for (int neighbor : g_adjacencyList[current])
//    {
//        // Skip the visited ones
//        if (visited.find(neighbor) != visited.end())
//        {
//            continue;
//        }
//
//        bool canVisit = true;
//        bool nextUsedTicket = usedTicket;
//
//        // Odd neighbor requires the ticket
//        if (neighbor % 2 == 1)
//        {
//            if (usedTicket)
//            {
//                canVisit = false;
//            }
//            else
//            {
//                nextUsedTicket = true;
//            }
//        }
//
//        if (canVisit)
//        {
//            Recur_TraverseDFS(neighbor, visited, outMaxCities, nextUsedTicket);
//        }
//    }
//
//    visited.erase(current);
//}
//
//int solution(vector<int>& T)
//{
//    // Form the graph
//    g_adjacencyList.clear();
//    g_adjacencyList.resize(T.size());
//
//    // Skip the T[0], build the adjacency list
//    for (int i = 1; i < (int)T.size(); ++i)
//    {
//        g_adjacencyList[i].emplace_back(T[i]);
//        g_adjacencyList[T[i]].emplace_back(i);
//    }
//
//    // Run DFS from T[0]. Find the max cities count
//    int maxCitiesCount = 1;
//    bool usedTicket = false;
//    std::unordered_set<int> visited;
//    Recur_TraverseDFS(0, visited, maxCitiesCount, usedTicket);
//
//    return maxCitiesCount;
//}
