// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <memory>

export module UnitTests_SharedPtr;

import jpt.Utilities;
import jpt.SharedPtr;

bool UnitTests_SharedPtr_Char()
{
    jpt::SharedPtr<char> sharedCharPtr1 = jpt::MakeStrong<char>('C');
    JPT_ENSURE(sharedCharPtr1.IsValid());
    JPT_ENSURE(sharedCharPtr1);
    JPT_ENSURE(*sharedCharPtr1 == 'C');
    
    jpt::SharedPtr<char> sharedCharPtr2 = sharedCharPtr1;
    JPT_ENSURE(sharedCharPtr2.IsValid());
    JPT_ENSURE(sharedCharPtr2);
    JPT_ENSURE(*sharedCharPtr2 == 'C');

    jpt::SharedPtr<char> sharedCharPtr3 = sharedCharPtr1;
    JPT_ENSURE(sharedCharPtr3.IsValid());
    JPT_ENSURE(sharedCharPtr3);
    JPT_ENSURE(*sharedCharPtr3 == 'C');

    JPT_ENSURE(sharedCharPtr1.GetRefCount() == 3);
    JPT_ENSURE(sharedCharPtr2.GetRefCount() == 3);
    JPT_ENSURE(sharedCharPtr3.GetRefCount() == 3);

    sharedCharPtr3.Reset(new char('A'));
    JPT_ENSURE(sharedCharPtr3.IsValid());
    JPT_ENSURE(sharedCharPtr3);
    JPT_ENSURE(*sharedCharPtr3 == 'A');

    JPT_ENSURE(sharedCharPtr1.GetRefCount() == 2);
    JPT_ENSURE(sharedCharPtr2.GetRefCount() == 2);
    JPT_ENSURE(sharedCharPtr3.GetRefCount() == 1);

    sharedCharPtr1 = sharedCharPtr3;
    JPT_ENSURE(sharedCharPtr1.IsValid());
    JPT_ENSURE(sharedCharPtr1);
    JPT_ENSURE(*sharedCharPtr1 == 'A');

    JPT_ENSURE(sharedCharPtr1.GetRefCount() == 2);
    JPT_ENSURE(sharedCharPtr2.GetRefCount() == 1);
    JPT_ENSURE(sharedCharPtr3.GetRefCount() == 2);

    jpt::SharedPtr<int> left(new int (3));
    jpt::SharedPtr<int> right(new int (4));
    jpt::SharedPtr<int> third(new int (5));
    left = right;
    third = jpt::Move(left);
    right = third;
    right = left;
    third.Reset(nullptr, [](int* pPtr)
        { 
            //JPT_LOG("Deleted a strong ptr");
            JPT_DELETE(pPtr); 
        });
    left.Reset(new int(42));
    third = left;
    right = jpt::Move(left);

    return true;
}

bool UnitTests_SharedPtr_Class()
{
    struct Bar;

    struct Foo
    {
        jpt::SharedPtr<Bar> m_bar;
    };

    struct Bar
    {
        jpt::WeakPtr<Foo> m_foo;
    };

    jpt::SharedPtr<Foo> foo(new Foo);
    jpt::SharedPtr<Bar> bar(new Bar);

    foo->m_bar = bar;
    bar->m_foo = foo;

    return true;
}

bool UnitTests_SharedPtr_Class2()
{
    struct Bar;
    
    struct Foo
    {
        jpt::WeakPtr<Bar> m_bar;
    };
    struct Bar
    {
        jpt::SharedPtr<Foo> m_foo;
    };

    jpt::SharedPtr<Foo> foo(new Foo);
    jpt::SharedPtr<Bar> bar(new Bar);

    foo->m_bar = bar;
    bar->m_foo = foo;

    foo.Reset(new Foo);

    jpt::WeakPtr<Foo> weakFoo;
    jpt::WeakPtr<Foo> weakFoo2;
    jpt::SharedPtr<Foo> sharedFoo1(new Foo);
    jpt::SharedPtr<Foo> sharedFoo2(new Foo);
    weakFoo = sharedFoo1;
    weakFoo = sharedFoo2;
    weakFoo = weakFoo2;
    weakFoo2 = sharedFoo1;
    *weakFoo2;

    return true;
}

export bool RunUnitTests_SharedPtr()
{
    JPT_ENSURE(UnitTests_SharedPtr_Char());
    JPT_ENSURE(UnitTests_SharedPtr_Class());
    JPT_ENSURE(UnitTests_SharedPtr_Class2());

    return true;
}