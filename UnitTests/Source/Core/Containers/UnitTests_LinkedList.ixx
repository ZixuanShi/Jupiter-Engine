// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_LinkedList;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;
import jpt.LinkedList;

bool UnitTest_LinkedList_Trivial()
{
    jpt::LinkedList<int32> list{ 7,8,9 };

    list.PushBack(1);
    list.PushFront(0);
    list.PushBack(2);
    list.PushFront(-1);

    list.InsertBefore(list.begin() + 2, 42);
    list.InsertAfter(list.begin() + (list.Size() - 1), 19);

    JPT_LOG(list);

    jpt::LinkedList<int32> anotherList{ -1, 0, 42, 7, 8, 9, 1, 2, 19 };
    JPT_RETURN_FALSE_IF_ERROR(list != anotherList, "");

    return true;
}

bool UnitTest_LinkedList_NonTrivial()
{
    //jpt::LinkedList<jpt::String> list;
    
    

    return true;
}

export bool RunUnitTests_LinkedList()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_LinkedList_Trivial(), "UnitTest_LinkedList_Trivial Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_LinkedList_NonTrivial(), "UnitTest_LinkedList_NonTrivial Failed");

    return true;
}
