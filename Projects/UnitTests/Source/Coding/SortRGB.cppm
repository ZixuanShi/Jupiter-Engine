// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Coding.SortRGB;

import jpt.TestFramework;
import jpt.TypeDefs;
import std;

namespace jpt
{
    enum class Color : uint8
    {
        R,
        G,
        B,
    };

    /** Sort R before G before B in one pass, partitioning around a shrinking unsorted middle:
        R swaps down to the front, B swaps up to the back, G is left where it lands. */
    void SortRGB(std::vector<Color>& colors)
    {
        if (colors.empty())
        {
            return;
        }

        usize sortedStartIndex = 0;
        usize sortedEndIndex = colors.size() - 1;

        usize i = 0;
        while (i <= sortedEndIndex)
        {
            if (colors[i] == Color::B)
            {
                std::swap(colors[i], colors[sortedEndIndex]);
                --sortedEndIndex;
            }

            if (colors[i] == Color::R)
            {
                std::swap(colors[i], colors[sortedStartIndex]);
                ++sortedStartIndex;
            }

            ++i;
        }
    }

    // By value: the parameter's copy is the fixture, so a case is one expression
    std::vector<Color> Sorted(std::vector<Color> colors)
    {
        SortRGB(colors);
        return colors;
    }

    void Test(TestCase& test)
    {
        using enum Color;
        using Colors = std::vector<Color>;

        test.Expect(Sorted({}) == Colors{}, "empty");
        test.Expect(Sorted({ G }) == Colors{ G }, "single");
        test.Expect(Sorted({ B, G, R }) == Colors{ R, G, B }, "reversed");
        test.Expect(Sorted({ R, R, G, G, B }) == Colors{ R, R, G, G, B }, "already sorted");
        test.Expect(Sorted({ B, B, B }) == Colors{ B, B, B }, "one color only");
        test.Expect(Sorted({ B, R, B, R }) == Colors{ R, R, B, B }, "missing middle color");
        test.Expect(Sorted({ G, G, R }) == Colors{ R, G, G }, "missing last color");
        test.Expect(Sorted({ G, B, B, R, G, R, B, B, R }) == Colors{ R, R, R, G, G, B, B, B, B }, "mixed");

        // Sorted() copies, so only a named vector can show the sort happened in place
        Colors colors{ B, R, G };
        const Color* pData = colors.data();
        SortRGB(colors);
        test.Expect(colors == Colors{ R, G, B } && colors.data() == pData, "sorted in place, no reallocation");
    }

    static TestCase s_sortRGB("Coding.SortRGB", &Test);
}
