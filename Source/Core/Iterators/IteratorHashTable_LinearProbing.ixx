// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.IteratorHashTable_LinearProbing;

import jpt.TypeDefs;
import jpt.Pair;

export namespace jpt_private
{
    template<typename TData>
    class IteratorHashTable_LinearProbing
    {
    public:
     
     
    private:
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr IteratorHashTable_LinearProbing() = default;
    };

    template<typename TData>
    class ConstIteratorHashTable_LinearProbing
    {
    public:

    private:
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr ConstIteratorHashTable_LinearProbing() = default;
    };
}