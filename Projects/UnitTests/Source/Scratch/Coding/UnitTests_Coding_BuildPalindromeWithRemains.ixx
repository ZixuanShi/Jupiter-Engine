// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_BuildPalindromeWithRemains;

import jpt.Utilities;
import jpt.TypeDefs;
import jpt.String;
import jpt.HashMap;

// Calculate the minimum number of letters to be removed, that the remaining letters can build a palindrome. Rearranging is allowed.
// @example:
// "abca" -> "aba" -> 1
// "abcda" -> "aba" -> 2
// "ervervige" -> "reviver" -> 2
size_t BuildPalindromeWithRemains(const jpt::String& str)
{
    // Count the frequency of each letter
    jpt::HashMap<char, size_t> frequency;
    for (char c : str)
    {
        ++frequency[c];
    }

    // Count the number of odd frequency letters
    size_t oddCount = 0;
    for (const auto& [_, count] : frequency)
    {
        JPT_IGNORE(_);
        if (count % 2 == 1)
        {
            ++oddCount;
        }
    }

    // if no odd frequency letter, then the string is already a palindrome
    if (oddCount == 0)
    {
        return 0;
    }

    // if there is only one odd frequency letter, then it can be placed in the middle of the palindrome
    return oddCount - 1;
}

export bool UnitTests_Coding_BuildPalindromeWithRemains()
{
    if (BuildPalindromeWithRemains("abca") != 1)
    {
        JPT_LOG("BuildPalindromeWithRemains abca failed");
    }

    if (BuildPalindromeWithRemains("abcda") != 2)
    {
        JPT_LOG("BuildPalindromeWithRemains abcda failed");
    }

    if (BuildPalindromeWithRemains("ervervige") != 2)
    {
        JPT_LOG("BuildPalindromeWithRemains ervervige failed");
    }

    return true;
}

//int solution(string& S)
//{
//    // Find the frequency for each character
//    std::unordered_map<char, int> frequency;
//    for (char c : S)
//    {
//        ++frequency[c];
//    }
//
//    // Count the number of odd letters
//    int oddCount = 0;
//    for (auto itr = frequency.begin(); itr != frequency.end(); ++itr)
//    {
//        if (itr->second % 2 == 1)
//        {
//            ++oddCount;
//        }
//    }
//
//    // If no odd frequency letter, it's already a palindrome
//    if (oddCount == 0)
//    {
//        return 0;
//    }
//
//    // If thre's only 1 odd frequency letter, it can be placed in the middle of the palindrome
//    return oddCount - 1;
//}
