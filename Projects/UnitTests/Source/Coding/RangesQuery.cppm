// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Coding.RangesQuery;

import jpt.TestFramework;
import jpt.TypeDefs;
import std;

namespace jpt
{
    /** A collection of ranges with two operations:

        Insert(start, end): add the closed range [start, end] to the collection.
        Query(value): true if any range in the collection contains value. */
    class RangesQuery
    {
    private:
        using Range = std::pair<float64, float64>;

        std::vector<Range> m_ranges;

    public:
        RangesQuery() = default;

    public:
        void Insert(float64 start, float64 end);

        [[nodiscard]] bool Query(float64 value) const;
    };

    void RangesQuery::Insert(float64 start, float64 end)
    {
        // Empty, and storing it would break the sorted-disjoint invariant the search relies on
        if (start > end)
        {
            return;
        }

        // Both bounds are sorted while the ranges are disjoint, so each end binary-searches its own.
        // [first, last) is every range this one touches, closed bounds included.
        const auto first = std::ranges::lower_bound(m_ranges, start, std::ranges::less{}, &Range::second);
        const auto last = std::ranges::upper_bound(m_ranges, end, std::ranges::less{}, &Range::first);

        if (first != last)
        {
            start = std::min(start, first->first);
            end = std::max(end, std::prev(last)->second);
        }

        m_ranges.insert(m_ranges.erase(first, last), Range(start, end));
    }

    bool RangesQuery::Query(float64 value) const
    {
        // Disjoint ranges leave one candidate: the last one starting at or before value
        const auto it = std::ranges::upper_bound(m_ranges, value, std::ranges::less{}, &Range::first);

        return it != m_ranges.begin() && value <= std::prev(it)->second;
    }

    void Test(TestCase& test)
    {
        // Every bound here is exactly representable, so the comparisons are exact
        RangesQuery empty;
        test.Expect(!empty.Query(0.0), "nothing inserted, nothing contained");

        RangesQuery single;
        single.Insert(1.0, 5.0);
        test.Expect(single.Query(3.0), "inside");
        test.Expect(single.Query(1.0), "the start is contained");
        test.Expect(single.Query(5.0), "and so is the end -- the range is closed");
        test.Expect(!single.Query(0.75), "just short of the start");
        test.Expect(!single.Query(5.25), "just past the end");

        RangesQuery disjoint;
        disjoint.Insert(0.0, 1.0);
        disjoint.Insert(10.0, 11.0);
        test.Expect(disjoint.Query(0.5), "the first range");
        test.Expect(disjoint.Query(10.5), "a later range is searched too");
        test.Expect(!disjoint.Query(5.0), "the gap between them belongs to neither");

        // Overlap and nesting are a union, so a value covered twice is still one yes
        RangesQuery overlapping;
        overlapping.Insert(0.0, 6.0);
        overlapping.Insert(4.0, 10.0);
        test.Expect(overlapping.Query(5.0), "the overlap");
        test.Expect(overlapping.Query(9.0), "past the first range's end, still inside the second");

        RangesQuery nested;
        nested.Insert(0.0, 100.0);
        nested.Insert(40.0, 60.0);
        test.Expect(nested.Query(50.0), "inside both");
        test.Expect(nested.Query(90.0), "inside the outer one alone");
        test.Expect(!nested.Query(101.0), "outside both");

        RangesQuery touching;
        touching.Insert(0.0, 1.0);
        touching.Insert(1.0, 2.0);
        test.Expect(touching.Query(1.0), "closed ranges both own the endpoint they share");

        RangesQuery point;
        point.Insert(3.0, 3.0);
        test.Expect(point.Query(3.0), "a zero-width range contains its one value");
        test.Expect(!point.Query(2.9375), "and nothing on either side of it");

        RangesQuery inverted;
        inverted.Insert(5.0, 1.0);
        test.Expect(!inverted.Query(3.0), "start > end is empty, not reversed");
        test.Expect(!inverted.Query(5.0) && !inverted.Query(1.0), "not even its own bounds");

        RangesQuery negatives;
        negatives.Insert(-5.0, -1.0);
        test.Expect(negatives.Query(-3.0), "negative bounds order the same way");
        test.Expect(negatives.Query(-5.0) && negatives.Query(-1.0), "both ends closed");
        test.Expect(!negatives.Query(0.0), "and the range does not reach zero");

        RangesQuery duplicate;
        duplicate.Insert(1.0, 2.0);
        duplicate.Insert(1.0, 2.0);
        test.Expect(duplicate.Query(1.5), "re-inserting a range changes no answer");
        test.Expect(!duplicate.Query(2.5), "including outside it");

        // Insert order does not matter, and one range can absorb several at once
        RangesQuery bridging;
        bridging.Insert(10.0, 11.0);
        bridging.Insert(0.0, 1.0);
        bridging.Insert(5.0, 6.0);
        test.Expect(bridging.Query(5.5) && !bridging.Query(3.0), "inserted back to front, still three separate ranges");
        test.Expect(bridging.Query(0.5) && bridging.Query(10.5), "and every one is found wherever it was inserted");
        bridging.Insert(0.5, 10.5);
        test.Expect(bridging.Query(3.0), "a range spanning the gaps merges all three");
        test.Expect(bridging.Query(0.0) && bridging.Query(11.0), "and the merged range keeps the outer bounds");
        test.Expect(!bridging.Query(11.5), "without reaching past them");
    }

    static TestCase s_rangesQuery("Coding.RangesQuery", &Test);
}
