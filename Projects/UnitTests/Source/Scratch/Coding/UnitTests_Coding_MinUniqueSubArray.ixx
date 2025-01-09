// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_Coding_MinUniqueSubArray;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Constants;
import jpt.HashSet;
import jpt.Math;

//int32 GetMinScore(const jpt::DynamicArray<Id>& watchHistory, Id series1, Id series2)
//{
//	if (series1 == series2)
//	{
//		return 1;
//	}
//
//	int32 minScore = jpt::Constants<int32>::kMax;
//
//	for (size_t beginIndex = 0; beginIndex < watchHistory.Count(); ++beginIndex)
//	{
//		for (size_t endIndex = beginIndex + 1; endIndex < watchHistory.Count(); ++endIndex)
//		{
//			bool hasSeries1 = false;
//			bool hasSeries2 = false;
//
//			for (size_t i = beginIndex; i <= endIndex; ++i)
//			{
//				if (watchHistory[i] == series1)
//				{
//					hasSeries1 = true;
//				}
//				if (watchHistory[i] == series2)
//				{
//					hasSeries2 = true;
//				}
//			}
//
//			if (hasSeries1 && hasSeries2)
//			{
//				jpt::HashSet<Id> shows;
//				for (size_t i = beginIndex; i <= endIndex; ++i)
//				{
//					shows.Add(watchHistory[i]);
//				}
//
//				minScore = jpt::Min(minScore, static_cast<int32>(shows.Count()));
//			}
//		}
//	}
//
//	return minScore;
//}

int32 GetMinScore(const jpt::DynamicArray<Id>& watchHistory, Id series1, Id series2)
{
	if (series1 == series2)
	{
		return 1;
	}

	int32 minScore = jpt::Constants<int32>::kMax;

	// Find the subarrays that starts and ends with series 1 or 2
	size_t beginIndex = 0;
	size_t endIndex = 0;

	while (true)
	{
		bool isBeginSeries1 = false;

		// Find begin
		while (beginIndex < watchHistory.Count() &&
			watchHistory[beginIndex] != series1 && watchHistory[beginIndex] != series2)
		{
			++beginIndex;
		}
		if (beginIndex >= watchHistory.Count())
		{
			break;
		}
		if (watchHistory[beginIndex] == series1)
		{
			isBeginSeries1 = true;
		}

		// Find end
		endIndex = beginIndex + 1;
		if (isBeginSeries1)
		{
			while (endIndex < watchHistory.Count() &&
				watchHistory[endIndex] != series2)
			{
				++endIndex;
			}
		}
		else
		{
			while (endIndex < watchHistory.Count() &&
				watchHistory[endIndex] != series1)
			{
				++endIndex;
			}
		}
		if (endIndex >= watchHistory.Count())
		{
			break;
		}

		jpt::HashSet<Id> shows;
		for (size_t i = beginIndex; i <= endIndex; ++i)
		{
			shows.Add(watchHistory[i]);
		}

		minScore = jpt::Min(minScore, static_cast<int32>(shows.Count()));

		beginIndex = endIndex;
		++endIndex;
	}

	return minScore;
}

export bool UnitTests_Coding_MinUniqueSubArray()
{
	JPT_ENSURE(GetMinScore({ 1, 3, 2, 1, 4 }, 1, 2) == 2);
	JPT_ENSURE(GetMinScore({ 1, 2, 2, 2, 2, 5, 2 }, 1, 5) == 3);
	JPT_ENSURE(GetMinScore({ 1, 2, 2, 2, 2, 5, 1 }, 1, 5) == 2);
	JPT_ENSURE(GetMinScore({ 1, 2, 3, 4, 5 }, 1, 5) == 5);
	JPT_ENSURE(GetMinScore({ 1, 2, 3, 4, 5, 6, 1 }, 1, 5) == 3);

	return true;
}