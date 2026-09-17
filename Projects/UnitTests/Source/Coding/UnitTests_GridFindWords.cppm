// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.GridFindWords;

import UnitTests.TestFramework;
import jpt.Logger;
import jpt.TypeDefs;
import std;

/*
Given: 
- A matrix contains alphabet letters
- A dictionary class contains valid words

Constraints:
- The word must be formed connecting neighbor letters within the matrix
- A letter at the same index cannot be used more than once
- It must be a valid word in the dictionary

Task: 
- Find all valid words from the matrix

Example:

Matrix:
A B E T P
M A O U V
N C D E F
Y X L R K

Dictionary:
- BET
- MANY
- ABOUT
- LERK
- TEA
- TUESDAY
- TACO
- BAD

Output:
- BET
- MANY
- ABOUT
- LERK
- TEA
- BAD
*/

class Dictionary
{
    std::unordered_set<std::string> m_data;

public:
    // bool IsWord(std::string_view str) const
    // {
    //     return m_data.contains(str);
    // }
};

std::unordered_set<std::string> FindWords(const std::vector<std::vector<char>>& matrix, const Dictionary& dict)
{

}

export void RunUnitTests_GridFindWords(jpt::TestCase& test)
{
}