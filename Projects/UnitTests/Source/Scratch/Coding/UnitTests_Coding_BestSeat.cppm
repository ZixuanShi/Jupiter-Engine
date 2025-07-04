// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_Coding_BestSeat;

import jpt.Constants;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.Optional;

jpt::Optional<uint32> GetBestSeat(const jpt::DynamicArray<uint32>& seats)
{
    uint32 bestSeat = jpt::kInvalidValue<uint32>;
    int32 bestScore = jpt::Constants<uint32>::kMin;
    for (uint32 i = 0; i < seats.Count(); ++i)
    {
        if (seats[i] == 1)
        {
            continue;
        }

        int32 score = 0;
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

    if (bestSeat == jpt::kInvalidValue<uint32>)
    {
        return {};
    }

    return bestSeat;
}

export bool UnitTests_Coding_BestSeat()
{
    jpt::DynamicArray<uint32> seats;

    seats = { 1, 0, 1, 0, 0, 0, 1 };
    JPT_ENSURE(GetBestSeat(seats).Value() == 4);

    seats = { 1, 0, 1 };
    JPT_ENSURE(GetBestSeat(seats).Value() == 1);

    seats = { 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };
    JPT_ENSURE(GetBestSeat(seats).Value() == 10);

    seats = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 };
    JPT_ENSURE(GetBestSeat(seats).Value() == 3);

    seats = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    JPT_ENSURE(GetBestSeat(seats).Value() == 4);

    seats = { 1, 1, 1 };
    JPT_ENSURE(!GetBestSeat(seats).HasValue());

    return true;
}