// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <initializer_list>
#include <algorithm>

module jpt.RandWeighted;

namespace jpt
{
    void RandWeighted::Init(const std::initializer_list<Precision>& weights)
    {
        m_cumulativeWeights.Clear();
        m_cumulativeWeights.Reserve(weights.size());

        Precision totalWeight = 0.0f;
        for (Precision weight : weights)
        {
            totalWeight += weight;
            m_cumulativeWeights.EmplaceBack(totalWeight);
        }
    }

    Index RandWeighted::Select(RNG& rng) const
    {
        const Precision random = rng.RangedFloat(0.0f, m_cumulativeWeights.Back());

        // Binary search for the index of the first element greater than or equal to random
        const auto it = std::lower_bound(m_cumulativeWeights.begin(), m_cumulativeWeights.end(), random);
        return static_cast<Index>(it - m_cumulativeWeights.begin());
    }
}