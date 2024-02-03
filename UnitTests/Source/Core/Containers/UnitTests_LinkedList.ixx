// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_LinkedList;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;
import jpt.LinkedList;

auto locHelper = [](size_t i) -> const char*
    {
        switch (i)
        {
        case 0: return "Zero";
        case 1: return "One";
        case 2: return "Two";
        case 3: return "Three";
        case 4: return "Four";
        case 5: return "Five";
        case 6: return "Six";
        case 7: return "Seven";
        case 8: return "Eight";
        case 9: return "Nine";
        case 10: return "Ten";
        case 11: return "Eleven";
        case 12: return "Twelve";

        default: return "Error";
        }
    };

bool UnitTest_LinkedList_Trivial()
{
    jpt::LinkedList<int32> list{ 7,8,9 };

    int32& result1 = list.EmplaceBack(1);
    JPT_RETURN_FALSE_IF_ERROR(result1 != 1, "");
    result1 = 12;
    int32& result2 = list.EmplaceFront(0);
    JPT_RETURN_FALSE_IF_ERROR(result2 != 0, "");
    result2 = 5;
    list.PushBack(2);
    list.PushFront(-1);

    list.EmplaceAfter(list.begin() + 3, 1);
    list.InsertBefore(list.begin() + 2, 3);
    list.InsertAfter(list.begin() + (list.Size() - 1), 11);
    list.EmplaceBefore(list.begin() + 5, 0);

    list.PopBack();
    list.PopFront();

    jpt::LinkedList<int32> anotherList{ 5, 3, 7, 8, 0, 1, 9, 12, 2 };
    JPT_RETURN_FALSE_IF_ERROR(list != anotherList, "");

    jpt::LinkedList<int32> copiedList = anotherList;
    JPT_RETURN_FALSE_IF_ERROR(list != anotherList, "");
    JPT_RETURN_FALSE_IF_ERROR(list != copiedList, "");

    jpt::LinkedList<int32> movedList = jpt::Move(anotherList);
    JPT_RETURN_FALSE_IF_ERROR(list != movedList, "");
    JPT_RETURN_FALSE_IF_ERROR(list != copiedList, "");
    JPT_RETURN_FALSE_IF_ERROR(!anotherList.IsEmpty(), "");

    return true;
}

bool UnitTest_LinkedList_NonTrivial()
{
    jpt::LinkedList<jpt::String> list{ "Seven", "Eight", "Nine" };

    jpt::String& result1 = list.EmplaceBack(locHelper(1));
    JPT_RETURN_FALSE_IF_ERROR(result1 != locHelper(1), "");
    result1 = locHelper(12);
    jpt::String& result2 = list.EmplaceFront(locHelper(0));
    JPT_RETURN_FALSE_IF_ERROR(result2 != locHelper(0), "");
    result2 = locHelper(5);
    list.PushBack(locHelper(2));
    list.PushFront(locHelper(1));

    list.EmplaceAfter(list.begin() + 3, locHelper(1));
    list.InsertBefore(list.begin() + 2, locHelper(3));
    list.InsertAfter(list.begin() + (list.Size() - 1), locHelper(11));
    list.EmplaceBefore(list.begin() + 5, locHelper(0));

    list.PopBack();
    list.PopFront();

    jpt::LinkedList<jpt::String> anotherList{ locHelper(5), locHelper(3),locHelper(7),locHelper(8),locHelper(0),locHelper(1),locHelper(9),locHelper(12),locHelper(2) };
    JPT_RETURN_FALSE_IF_ERROR(list != anotherList, "");

    jpt::LinkedList<jpt::String> copiedList = anotherList;
    JPT_RETURN_FALSE_IF_ERROR(list != anotherList, "");
    JPT_RETURN_FALSE_IF_ERROR(list != copiedList, "");

    jpt::LinkedList<jpt::String> movedList = jpt::Move(anotherList);
    JPT_RETURN_FALSE_IF_ERROR(list != movedList, "");
    JPT_RETURN_FALSE_IF_ERROR(list != copiedList, "");
    JPT_RETURN_FALSE_IF_ERROR(!anotherList.IsEmpty(), "");

    return true;
}

export bool RunUnitTests_LinkedList()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_LinkedList_Trivial(), "UnitTest_LinkedList_Trivial Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_LinkedList_NonTrivial(), "UnitTest_LinkedList_NonTrivial Failed");

    return true;
}
