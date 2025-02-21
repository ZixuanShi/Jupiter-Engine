// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_Coding_BestMeetingNode;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Optional;
import jpt.Utilities;
import jpt.Pair;

// Given a graph. 
// N people are standing at different nodes. 
// Find the best meeting node that minimizes the total distance traveled by all people.
// The graph is directed and weighted.

using Edge           = jpt::Pair<Index, float32>;
using Neighbors      = jpt::DynamicArray<Edge>;
using AdjencencyList = jpt::DynamicArray<Neighbors>;

export bool UnitTests_Coding_BestMeetingNode()
{
	return true;
}