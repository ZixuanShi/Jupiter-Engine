// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_SetBitsInRange;

import jpt.TestFramework;
import jpt.TypeDefs;

/** Read-modify-write of one bit field: original[toBit:fromBit] = value[toBit - fromBit : 0].

    The mask is built by shrinking all-ones to the field's width and then sliding it into
    place, which keeps every shift count inside [0, 31]. The obvious (1u << width) - 1 shifts
    by 32 when the field is the whole register. */
uint32 SetBitsInRange(uint32 original, int32 fromBit, int32 toBit, uint32 value)
{
    uint32 mask = ~0u;
    mask >>= (31 - toBit + fromBit);    // narrow to the field's width
    mask <<= fromBit;                   // slide it onto the field

    value <<= fromBit;
    value &= mask;                      // drop whatever is wider than the field

    original &= ~mask;                  // wipe the field
    original |= value;

    return original;
}

export void RunUnitTests_SetBitsInRange(jpt::TestCase& test)
{
    test.Expect(SetBitsInRange(0b1011011101111, 3, 7, 0b01010) == 0b1011001010111, "slide example");
    test.Expect(SetBitsInRange(0b1011011101111, 3, 7, 0b1111111) == 0b1011011111111, "value wider than the field is truncated");
    test.Expect(SetBitsInRange(0xFFFFFFFF, 8, 15, 0) == 0xFFFF00FF, "bits outside the field are untouched");
    test.Expect(SetBitsInRange(0xDEADBEEF, 4, 11, 0xEE) == 0xDEADBEEF, "writing the field back changes nothing");
    test.Expect(SetBitsInRange(0xAAAAAAAA, 0, 31, 0x12345678) == 0x12345678, "the whole register");
    test.Expect(SetBitsInRange(0, 31, 31, 1) == 0x80000000, "the top bit alone");
    test.Expect(SetBitsInRange(0xFFFFFFFF, 0, 0, 0) == 0xFFFFFFFE, "the bottom bit alone");
}
