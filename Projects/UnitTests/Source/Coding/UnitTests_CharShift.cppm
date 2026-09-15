// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.CharShift;

import UnitTests.TestFramework;
import jpt.Assert;
import jpt.Constants;
import jpt.TypeDefs;
import std;

constexpr std::string_view kAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
constexpr int32 kAlphabetSize = static_cast<int32>(kAlphabet.size());

/** @return c's position in kAlphabet, or jpt::kInvalid<usize> if c is not part of it. */
constexpr int32 ToIndex(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return static_cast<int32>(c - 'a');
    }
    if (c >= 'A' && c <= 'Z')
    {
        return static_cast<int32>(c - 'A') + 26;
    }
    if (c >= '0' && c <= '9')
    {
        return static_cast<int32>(c - '0') + 52;
    }

    return jpt::kInvalid<int32>;
}

/** @return input with every character advanced num places along kAlphabet, wrapping at either
            end. input is [a-zA-Z0-9] only; anything else is a caller bug. */
std::string Shift(std::string_view input, int32 num)
{
    num %= kAlphabetSize;
    if (num < 0)
    {
        num += kAlphabetSize;
    }

    std::string result;
    result.reserve(input.size());

    for (char c : input)
    {
        const int32 sourceIndex = ToIndex(c);
        const int32 shiftIndex = (sourceIndex + num) % kAlphabetSize;
        const char newChar = kAlphabet[shiftIndex];
        result.push_back(newChar);
    }

    return result;
}

export void RunUnitTests_CharShift(jpt::TestCase& test)
{
    test.Expect(Shift("abcdef",  1) == "bcdefg", "shift right by 1");
    test.Expect(Shift("abcdef",  2) == "cdefgh", "shift right by 2");
    test.Expect(Shift("azAZ09",  1) == "bAB01a", "shift right by 1");
    test.Expect(Shift("azAZ09", -1) == "9yzYZ8", "shift left by -1");

    // Empty input, and a shift that moves nothing
    test.Expect(Shift("", 7) == "", "an empty input stays empty");
    test.Expect(Shift("azAZ09", 0) == "azAZ09", "a shift of 0 is the identity");

    // Every seam of the cycle, both directions
    test.Expect(Shift("zZ9",  1) == "A0a", "each class rolls into the next");
    test.Expect(Shift("aA0", -1) == "9zZ", "and back into the previous one");
    test.Expect(Shift("abcABC", 26) == "ABC012", "26 advances a whole class");

    // Every character of the alphabet at once, rotated one step each way
    test.Expect(Shift(kAlphabet,  1) == "bcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789a", "the whole alphabet, rotated left");
    test.Expect(Shift(kAlphabet, -1) == "9abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345678", "the whole alphabet, rotated right");

    // num far outside one cycle, in both directions
    test.Expect(Shift("Jupiter",   63) == "Kvqjufs", "a shift past the end of the cycle wraps");
    test.Expect(Shift("Jupiter",  -61) == "Kvqjufs", "-61 is the same walk as +1");
    test.Expect(Shift("Jupiter",  620) == "Jupiter", "a whole number of cycles changes nothing");
    test.Expect(Shift("Jupiter", -620) == "Jupiter", "and neither does it backwards");

    // The extremes of int32, where naive negation or c + num overflows
    test.Expect(Shift("abcdef", std::numeric_limits<int32>::max()) == "bcdefg", "INT32_MAX is one step past 34636833 cycles");
    test.Expect(Shift("abcdef", std::numeric_limits<int32>::min()) == "89abcd", "INT32_MIN is two steps short of one");

    // Round trips
    constexpr std::string_view kSample = "JupiterEngine2026";
    test.Expect(Shift(Shift(kSample, 37), -37) == kSample, "shifting back by the same amount restores the input");
    test.Expect(Shift(Shift(kSample, 31),  31) == kSample, "two half cycles make a whole one");
}
