// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InputTests;

import jpt.EventDispatcher;
import jpt.TypeDefs;

export namespace jpt
{
    /** Runs at startup outside Release. Synthetic key and mouse events need assistive access,
        so everything below the OS is asserted rather than clicked. */
    void RunInputTests();
}

/** Not exported: the fixture EventDispatcher is exercised with, carrying no meaning outside
    the assertions in InputTests.cpp. */
namespace jpt::local
{
    struct TestEvent
    {
        int32 value = 0;
    };

    using TestDispatcher = EventDispatcher<TestEvent>;

    struct TestListener
    {
        int32 count = 0;

        void OnEvent(const TestEvent& event);
    };
}
