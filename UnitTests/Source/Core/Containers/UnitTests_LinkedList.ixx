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
    jpt::LinkedList<int32> list;

    list.PushBack(0);
    list.PushBack(1);
    list.PushBack(2);

    JPT_LOG(list);

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
