// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EventDispatcherTests;

import jpt.Constants;
import jpt.EventDispatcher;
import jpt.TestFramework;
import jpt.TypeDefs;

/** The fixture EventDispatcher is exercised with, carrying no meaning outside these assertions. */
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

    void TestListener::OnEvent(const TestEvent&)
    {
        ++count;
    }

    void RegistrationTest(TestCase& test)
    {
        TestDispatcher dispatcher;

        int32 firstCount = 0;
        int32 secondCount = 0;

        const TestDispatcher::Handle first = dispatcher.Add([&firstCount](const TestEvent&) { ++firstCount; });
        const TestDispatcher::Handle second = dispatcher.Add([&secondCount](const TestEvent&) { ++secondCount; });

        test.Expect(first != second, "Add returned the same handle twice");
        test.Expect(first != kInvalid<TestDispatcher::Handle>, "Add returned the invalid handle");
        test.Expect(dispatcher.GetCount() == 2, "Dispatcher holds {} handlers, expected 2", dispatcher.GetCount());

        dispatcher.Dispatch(TestEvent{});
        test.Expect(firstCount == 1 && secondCount == 1, "Dispatch reached {} and {}, expected 1 and 1", firstCount, secondCount);

        // Remove must stop exactly one handler.
        dispatcher.Remove(first);
        dispatcher.Dispatch(TestEvent{});
        test.Expect(firstCount == 1, "Removed handler still fired, count is {}", firstCount);
        test.Expect(secondCount == 2, "Surviving handler stopped firing, count is {}", secondCount);

        // Removing an unknown handle is a no-op, not a crash or a silent extra tombstone.
        dispatcher.Remove(first);
        test.Expect(dispatcher.GetCount() == 1, "Repeated Remove changed the count to {}", dispatcher.GetCount());
    }

    void MemberBindingTest(TestCase& test)
    {
        TestDispatcher dispatcher;
        TestListener listener;

        const TestDispatcher::Handle handle = dispatcher.Add(&listener, &TestListener::OnEvent);
        dispatcher.Dispatch(TestEvent{});
        test.Expect(listener.count == 1, "Member handler fired {} times, expected 1", listener.count);

        dispatcher.Remove(handle);
        dispatcher.Dispatch(TestEvent{});
        test.Expect(listener.count == 1, "Removed member handler still fired, count is {}", listener.count);
    }

    /** Legacy's Send was undefined behaviour here. */
    void ReentrancyTest(TestCase& test)
    {
        {
            TestDispatcher dispatcher;

            // Removing itself destroys the std::function it is executing in, unless Remove defers.
            int32 selfCount = 0;
            TestDispatcher::Handle self = kInvalid<TestDispatcher::Handle>;
            self = dispatcher.Add([&dispatcher, &selfCount, &self](const TestEvent&)
                {
                    ++selfCount;
                    dispatcher.Remove(self);
                });

            dispatcher.Dispatch(TestEvent{});
            dispatcher.Dispatch(TestEvent{});
            test.Expect(selfCount == 1, "Self-removing handler fired {} times, expected 1", selfCount);
        }
        {
            TestDispatcher dispatcher;

            // Adding grows the container; the new handler must not see the in-flight event.
            int32 addedCount = 0;
            int32 adderCount = 0;
            dispatcher.Add([&dispatcher, &addedCount, &adderCount](const TestEvent&)
                {
                    ++adderCount;
                    if (adderCount == 1)
                    {
                        dispatcher.Add([&addedCount](const TestEvent&) { ++addedCount; });
                    }
                });

            dispatcher.Dispatch(TestEvent{});
            test.Expect(addedCount == 0, "Handler added mid-dispatch received the in-flight event");

            dispatcher.Dispatch(TestEvent{});
            test.Expect(addedCount == 1, "Handler added mid-dispatch never fired, count is {}", addedCount);
        }
    }

    static TestCase s_registration("Core.EventDispatcherRegistration", &RegistrationTest);
    static TestCase s_memberBinding("Core.EventDispatcherMemberBinding", &MemberBindingTest);
    static TestCase s_reentrancy("Core.EventDispatcherReentrancy", &ReentrancyTest);
}
