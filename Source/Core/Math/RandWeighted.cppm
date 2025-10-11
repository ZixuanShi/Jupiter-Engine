// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.RandWeighted;

import jpt.Rand;
import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt
{
    /** Binary search based, cached, weighted random */
    class RandWeighted
    {
    private:
        DynamicArray<Precision> m_cumulativeWeights;

    public:
        void Init(const std::initializer_list<Precision>& weights);
        Index Select(RNG& rng) const;
    };
}