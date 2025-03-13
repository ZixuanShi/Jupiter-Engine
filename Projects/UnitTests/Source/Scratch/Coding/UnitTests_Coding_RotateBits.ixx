module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <memory>

export module UnitTests_Coding_RotateBits;

import jpt.Utilities;
import jpt.TypeDefs;
import jpt.Concepts;

constexpr uint8 RotateLeft(uint8 input, uint8 count) noexcept
{
    // Step 1. Extract the rotated bits
    const uint8 rotated = input >> (8 - count);

    // Step 2. Shift the input to the left
    input <<= count;

    // Step 3. Insert the rotated bits back into the input
    input |= rotated;

    return input;
}

constexpr uint8 RotateRight(uint8 input, uint8 count) noexcept
{
    const uint8 rotated = input << (8 - count);

    input >>= count;

    input |= rotated;

    return input;
}

export bool UnitTests_Coding_RotateBits()
{
    JPT_ENSURE(RotateLeft(0b10110011, 1) == 0b01100111);
    JPT_ENSURE(RotateLeft(0b10110011, 2) == 0b11001110);
    JPT_ENSURE(RotateLeft(0b10110011, 3) == 0b10011101);
    JPT_ENSURE(RotateLeft(0b10110011, 4) == 0b00111011);

    JPT_ENSURE(RotateRight(0b10110011, 1) == 0b11011001);
    JPT_ENSURE(RotateRight(0b10110011, 2) == 0b11101100);
    JPT_ENSURE(RotateRight(0b10110011, 3) == 0b01110110);
    JPT_ENSURE(RotateRight(0b10110011, 4) == 0b00111011);

    return true;
}