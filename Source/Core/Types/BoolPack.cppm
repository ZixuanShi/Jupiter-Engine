// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.BoolPack;

import jpt.TypeDefs;

export namespace jpt
{
    /** Packs 8 booleans within 1 byte */
    class BoolPack
    {
    private: 
        uint8 m_pack = 0;

    public:
        constexpr void Set(Index index, bool value) noexcept;
        constexpr bool Get(Index index) const noexcept;
        constexpr bool operator[](Index index) const noexcept;
    };

    constexpr void BoolPack::Set(Index index, bool value) noexcept
    {
        JPT_ASSERT(index < 8, "BoolPack can only hold 8 bools");
        if (value)
        {
            m_pack |= (1 << index);
        }
        else
        {
            m_pack &= ~(1 << index);
        }
    }

    constexpr bool BoolPack::Get(Index index) const noexcept
    {
        JPT_ASSERT(index < 8, "BoolPack can only hold 8 bools");
        return (m_pack & (1 << index));
    }

    constexpr bool BoolPack::operator[](Index index) const noexcept
    {
        return Get(index);
    }
}