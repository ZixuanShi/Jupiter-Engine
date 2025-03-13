// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Math_MergeCloseVertices;

import jpt.DynamicArray;
import jpt.Vector3;
import jpt.Rand;
import jpt.HashSet;
import jpt.TypeDefs;
import jpt.Utilities;

jpt::DynamicArray<Vec3f> GetTestData()
{
    // Input data for each solutions
    jpt::DynamicArray<Vec3f> positions =
    {
        { 0.00f, 0.00f, 0.00f },
        { 0.00f, 0.01f, 0.00f },    // Expected to be merged

        { 2.00f, 2.00f, 2.00f },
        { 2.01f, 2.00f, 2.00f },    // Expected to be merged

        { 3.00f, 3.00f, 3.01f },
        { 3.00f, 3.00f, 3.00f },    // Expected to be merged

        { 4.00f, 4.00f, 4.00f },
        { 4.00f, 4.00f, 4.01f },    // Expected to be merged
        { 4.00f, 4.00f, 4.00f },

        // Expected to be kept
        { 15.0f, 10.0f,  9.0f },
        { 20.0f,  0.0f, 19.0f },
        { 15.0f, 90.0f, 22.0f },
        {  7.0f, 18.0f, 26.0f },
    };

    //// Add more random data. Comment out if you want to test with only the above data
    //jpt::RNG& rng = jpt::RNG::Global();
    //for (size_t i = 0; i < 10'000; ++i)
    //{
    //    const float x = rng.RangedFloat(0.0f, 100.0f);
    //    const float y = rng.RangedFloat(0.0f, 100.0f);
    //    const float z = rng.RangedFloat(0.0f, 100.0f);
    //    positions.EmplaceBack(x, y, z);
    //}

    return positions;
}

namespace jpt
{
    static constexpr uint64 Hash(const Vec3i& v)
    {
        return Hash(v.x) ^ Hash(v.y) ^ Hash(v.z);
    }
}

// Spatial Hashing
static void MergeCloseVertices(jpt::DynamicArray<Vec3f>& positions, float32 threshold)
{
    jpt::HashSet<Vec3i> cells;

    for (Index i = 0; i < positions.Count();)
    {
        const Vec3i cell = Vec3i{ static_cast<int32>(positions[i].x / threshold),
                                  static_cast<int32>(positions[i].y / threshold),
                                  static_cast<int32>(positions[i].z / threshold)};

        if (cells.Has(cell))
        {
            positions.Erase(i);
        }
        else
        {
            cells.Add(cell);
            ++i;
        }
    }
}

export bool UnitTests_Math_MergeCloseVertices()
{
    jpt::DynamicArray<Vec3f> positions = GetTestData();
    const float32 threshold = 0.1f;

    MergeCloseVertices(positions, threshold);

    return true;
}