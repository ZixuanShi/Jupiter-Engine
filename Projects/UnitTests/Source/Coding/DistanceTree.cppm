// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Coding.DistanceTree;

import jpt.TestFramework;
import jpt.TypeDefs;

namespace jpt::Coding
{
    /** A binary tree whose edges carry weights, so a distance is the sum along the path. */
    struct Node
    {
        Node* pLeft = nullptr;
        Node* pRight = nullptr;
        int32 distanceToLeft = 0;
        int32 distanceToRight = 0;
    };

    static constexpr int32 kUnreachable = -1;

    /** Summed edge weights from pCurrent down to pTarget, or kUnreachable if pTarget is not in
        pCurrent's subtree. */
    int32 DistanceDown(const Node* pCurrent, const Node* pTarget)
    {
        if (!pCurrent)
        {
            return kUnreachable;
        }

        if (pCurrent == pTarget)
        {
            return 0;
        }

        if (const int32 left = DistanceDown(pCurrent->pLeft, pTarget); left != kUnreachable)
        {
            return pCurrent->distanceToLeft + left;
        }

        if (const int32 right = DistanceDown(pCurrent->pRight, pTarget); right != kUnreachable)
        {
            return pCurrent->distanceToRight + right;
        }

        return kUnreachable;
    }

    /** Lowest node with both targets beneath it. A node that *is* one of the targets is its own
        answer, which is what makes an ancestor-descendant pair work. */
    const Node* FindCommonParent(const Node* pCurrent, const Node* pFrom, const Node* pTo)
    {
        if (!pCurrent || pCurrent == pFrom || pCurrent == pTo)
        {
            return pCurrent;
        }

        const Node* pLeft = FindCommonParent(pCurrent->pLeft, pFrom, pTo);
        const Node* pRight = FindCommonParent(pCurrent->pRight, pFrom, pTo);

        if (pLeft && pRight)
        {
            return pCurrent;
        }

        return pLeft ? pLeft : pRight;
    }

    /** Distance between any two nodes: the path bends at their lowest common parent, so it is the
        two downward distances added. */
    int32 GetDistance(const Node* pRoot, const Node* pFrom, const Node* pTo)
    {
        const Node* pCommonParent = FindCommonParent(pRoot, pFrom, pTo);
        const int32 distanceToFrom = DistanceDown(pCommonParent, pFrom);
        const int32 distanceToTo = DistanceDown(pCommonParent, pTo);
        return distanceToFrom + distanceToTo;
    }

    void Test(TestCase& test)
    {
        /*        A
               3/   \4
              B       C
            4/ \6   1/ \2
           D    E   F    G
                  6/ \5
                  H    I      */
        Node a, b, c, d, e, f, g, h, i;

        a.pLeft  = &b; a.distanceToLeft = 3;
        a.pRight = &c; a.distanceToRight = 4;

        b.pLeft  = &d; b.distanceToLeft = 4;
        b.pRight = &e; b.distanceToRight = 6;

        c.pLeft  = &f; c.distanceToLeft = 1;
        c.pRight = &g; c.distanceToRight = 2;

        f.pLeft  = &h; f.distanceToLeft = 6;
        f.pRight = &i; f.distanceToRight = 5;

        test.Expect(GetDistance(&a, &a, &b) == 3, "root to child");
        test.Expect(GetDistance(&a, &a, &c) == 4, "root to other child");
        test.Expect(GetDistance(&a, &a, &h) == 11, "root to a leaf three levels down");
        test.Expect(GetDistance(&a, &b, &b) == 0, "a node to itself");
        test.Expect(GetDistance(&a, &f, &g) == 3, "siblings");
        test.Expect(GetDistance(&a, &h, &g) == 9, "across a subtree");
        test.Expect(GetDistance(&a, &d, &i) == 17, "opposite leaves, through the root");
    }

    static TestCase s_distanceTree("Coding.DistanceTree", &Test);
}
