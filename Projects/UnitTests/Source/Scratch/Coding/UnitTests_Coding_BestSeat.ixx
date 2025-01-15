// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_Coding_BestSeat;

import jpt.Constants;
import jpt.TypeDefs;
import jpt.DynamicArray;

uint32 GetBestSeat(const jpt::DynamicArray<uint32>& seats)
{
	uint32 bestSeat = 0;
	uint32 bestScore = jpt::Constants<uint32>::kMin;
	for (uint32 i = 0; i < seats.Count(); ++i)
	{
		if (seats[i] == 1)
		{
			continue;
		}

		uint32 score = 0;
		uint32 current = i;
		while (current < seats.Count() && 
			   seats[current] == 0)
		{
			++score;
			++current;
		}

		if (bestScore < score)
		{
			bestSeat = i + (score / 2);
			bestScore = score;

			i += score;
		}
	}

	return bestSeat;
}

export bool UnitTests_Coding_BestSeat()
{
	jpt::DynamicArray<uint32> seats;

	seats = { 1, 0, 1, 0, 0, 0, 1 };
	JPT_ENSURE(GetBestSeat(seats) == 4);

	seats = { 1, 0, 1 };
	JPT_ENSURE(GetBestSeat(seats) == 1);

	seats = { 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };
	JPT_ENSURE(GetBestSeat(seats) == 10);

	return true;
}