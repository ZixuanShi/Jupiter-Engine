
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Coding_DistanceTree;

import jpt.CoreModules;

/**
         A
      3/   \4
     B       C
   4/ \6   1/ \2
  D    E   F    G
         6/ \5
         H   I
*/

struct Node
{
    Node* pLeft   = nullptr;
    Node* pRight  = nullptr;
    int distanceToLeft  = 0;
    int distanceToRight = 0;
    char letter = ' ';

    Node(char _letter)
        : letter(_letter)
    {
    }
};

static constexpr int kInvalid = -1;

int RecurDFS(const Node* pCurrent, const Node* pTarget)
{
    if (!pCurrent)
    {
        return kInvalid;
    }

    if (pCurrent == pTarget)
    {
        return 0;
    }

    const int left = RecurDFS(pCurrent->pLeft, pTarget);
    if (left != kInvalid)
    {
        return pCurrent->distanceToLeft + left;
    }

    const int right = RecurDFS(pCurrent->pRight, pTarget);
    if (right != kInvalid)
    {
        return pCurrent->distanceToRight + right;
    }

    return kInvalid;
}

const Node* FindCommonParent(const Node* pCurrent, const Node* pFrom, const Node* pTarget)
{
    if (pFrom == pTarget)
    {
        return pFrom;
    }

    if (!pCurrent || pCurrent == pFrom || pCurrent == pTarget)
    {
        return pCurrent;
    }

    const Node* pLeft  = FindCommonParent(pCurrent->pLeft, pFrom, pTarget);
    const Node* pRight = FindCommonParent(pCurrent->pRight, pFrom, pTarget);

    if (pLeft && pRight)
    {
        return pCurrent;
    }

    return pLeft ? pLeft : pRight;
}

int GetDistance(const Node* pRoot, const Node* pFrom, const Node* pTarget)
{
    const Node* pCommonParent = FindCommonParent(pRoot, pFrom, pTarget);
    const int distanceToLeft  = RecurDFS(pCommonParent, pFrom);
    const int distanceToRight = RecurDFS(pCommonParent, pTarget);
    
    return distanceToLeft + distanceToRight;
}

export bool UnitTests_Coding_DistanceTree()
{
    Node* A = new Node('A');
    Node* B = new Node('B');
    Node* C = new Node('C');
    Node* D = new Node('D');
    Node* E = new Node('E');
    Node* F = new Node('F');
    Node* G = new Node('G');
    Node* H = new Node('H');
    Node* I = new Node('I');

    A->pLeft  = B;  A->distanceToLeft  = 3;
    A->pRight = C;  A->distanceToRight = 4;

    B->pLeft  = D;  B->distanceToLeft  = 4;
    B->pRight = E;  B->distanceToRight = 6;

    C->pLeft  = F;  C->distanceToLeft  = 1;
    C->pRight = G;  C->distanceToRight = 2;

    F->pLeft  = H;  F->distanceToLeft  = 6;
    F->pRight = I;  F->distanceToRight = 5;

    JPT_ENSURE(GetDistance(A, A, B) == 3);
    JPT_ENSURE(GetDistance(A, A, C) == 4);
    JPT_ENSURE(GetDistance(A, A, H) == 11);
    JPT_ENSURE(GetDistance(A, B, B) == 0);
    JPT_ENSURE(GetDistance(A, F, G) == 3);
    JPT_ENSURE(GetDistance(A, H, G) == 9);
    JPT_ENSURE(GetDistance(A, D, I) == 17);

    delete A; A = nullptr;
    delete B; B = nullptr;
    delete C; C = nullptr;
    delete D; D = nullptr;
    delete E; E = nullptr;
    delete F; F = nullptr;
    delete G; G = nullptr;
    delete H; H = nullptr;
    delete I; I = nullptr;

    return true;
}