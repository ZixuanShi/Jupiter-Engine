// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Coding.QuadTree;

import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

namespace jpt
{
    /** Locates objects in a square region while the only question that can be asked is "does this
        square hold anything?" -- subdivide, prune every quadrant that answers no, and report the
        centre of each occupied cell at the depth limit. The prune is the whole algorithm: finding a
        lone object costs 17 probes where scanning all 256 leaf cells costs 256.

        Cells are half-open, [x, x + length) on both axes, so an object on the boundary between two
        siblings belongs to exactly one of them and is never reported twice. The cost of that is the
        far edge: an object at exactly x + length sits outside the queried region. */
    class QuadTreeSearch
    {
    public:
        static constexpr usize kMaxDepth = 4;       // leaf cells are length / 16

    private:
        std::vector<Vec2d> m_objects;
        usize m_probes = 0;

    public:
        QuadTreeSearch(std::vector<Vec2d> objects);

    public:
        [[nodiscard]] std::vector<Vec2d> QueryObjects(float64 x, float64 y, float64 length);

        /** Probes the last query spent, which is what pruning has to earn. */
        [[nodiscard]] usize GetProbeCount() const noexcept { return m_probes; }

    private:
        /** results is by reference, so a hit deep in the recursion costs no vector copies. */
        void RecurQueryObjects(float64 x, float64 y, float64 length, usize depth, std::vector<Vec2d>& results);

        /** The black box the search is built around -- a broadphase or a sensor in a real system,
            a scan of the field here. */
        [[nodiscard]] bool HasObject(float64 x, float64 y, float64 length);
    };

    QuadTreeSearch::QuadTreeSearch(std::vector<Vec2d> objects)
        : m_objects(std::move(objects))
    {
    }

    std::vector<Vec2d> QuadTreeSearch::QueryObjects(float64 x, float64 y, float64 length)
    {
        std::vector<Vec2d> results;
        m_probes = 0;

        RecurQueryObjects(x, y, length, 0, results);

        return results;
    }

    void QuadTreeSearch::RecurQueryObjects(float64 x, float64 y, float64 length, usize depth, std::vector<Vec2d>& results)
    {
        // Nothing in this quadrant, so nothing in any of its children
        if (!HasObject(x, y, length))
        {
            return;
        }

        // Precision limit, and the probe above says this cell is occupied
        if (depth >= kMaxDepth)
        {
            results.push_back(Vec2d(x + length / 2.0, y + length / 2.0));
            return;
        }

        const float64 half = length / 2.0;

        RecurQueryObjects(x,        y,        half, depth + 1, results);    // SW
        RecurQueryObjects(x + half, y,        half, depth + 1, results);    // SE
        RecurQueryObjects(x,        y + half, half, depth + 1, results);    // NW
        RecurQueryObjects(x + half, y + half, half, depth + 1, results);    // NE
    }

    bool QuadTreeSearch::HasObject(float64 x, float64 y, float64 length)
    {
        ++m_probes;

        return std::ranges::any_of(m_objects, [x, y, length](const Vec2d& object)
            {
                return object.x >= x && object.x < x + length &&
                       object.y >= y && object.y < y + length;
            });
    }

    void Test(TestCase& test)
    {
        // A 16-wide root divides into 1-wide leaf cells, so every centre lands on a .5 and compares exactly
        QuadTreeSearch empty({});
        test.Expect(empty.QueryObjects(0.0, 0.0, 16.0).empty(), "empty field finds nothing");
        test.Expect(empty.GetProbeCount() == 1, "and one probe prunes the entire search");

        QuadTreeSearch single({ Vec2d(1.2, 3.7) });
        test.Expect(single.QueryObjects(0.0, 0.0, 16.0) == std::vector<Vec2d>{ Vec2d(1.5, 3.5) }, "one object, reported at its cell centre");
        test.Expect(single.GetProbeCount() == 17, "found in 17 probes, not the 256 leaf cells");

        QuadTreeSearch sameCell({ Vec2d(1.2, 3.7), Vec2d(1.9, 3.1) });
        test.Expect(sameCell.QueryObjects(0.0, 0.0, 16.0) == std::vector<Vec2d>{ Vec2d(1.5, 3.5) }, "two objects in one cell collapse to one centre");

        // Traversal order, not input order: SW, SE, NW, NE at every level
        QuadTreeSearch corners({ Vec2d(15.2, 15.9), Vec2d(0.1, 0.1), Vec2d(15.5, 0.5), Vec2d(0.5, 15.5) });
        test.Expect(corners.QueryObjects(0.0, 0.0, 16.0) == std::vector<Vec2d>{ Vec2d(0.5, 0.5), Vec2d(15.5, 0.5), Vec2d(0.5, 15.5), Vec2d(15.5, 15.5) }, "results come back SW, SE, NW, NE");

        QuadTreeSearch outside({ Vec2d(-1.0, 2.0), Vec2d(20.0, 20.0) });
        test.Expect(outside.QueryObjects(0.0, 0.0, 16.0).empty(), "objects outside the region are not reported");

        // Half-open cells: the root's midpoint belongs to NE alone
        QuadTreeSearch midpoint({ Vec2d(8.0, 8.0) });
        test.Expect(midpoint.QueryObjects(0.0, 0.0, 16.0) == std::vector<Vec2d>{ Vec2d(8.5, 8.5) }, "an object on a shared boundary is reported once");

        QuadTreeSearch farEdge({ Vec2d(16.0, 4.0) });
        test.Expect(farEdge.QueryObjects(0.0, 0.0, 16.0).empty(), "and the far edge is outside, which is what buys that");

        // An 8-wide region from a non-zero origin: leaf cells are 0.5 wide
        QuadTreeSearch subRegion({ Vec2d(10.3, 12.7), Vec2d(1.0, 1.0) });
        test.Expect(subRegion.QueryObjects(8.0, 8.0, 8.0) == std::vector<Vec2d>{ Vec2d(10.25, 12.75) }, "a sub-region queries only itself");
    }

    static TestCase s_quadTree("Coding.QuadTree", &Test);
}
