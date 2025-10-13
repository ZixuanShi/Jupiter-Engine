// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module UnitTests_EventSystem;

import jpt.TypeDefs;
import jpt.Event;
import jpt.EventManager;

import jpt.Event_Mouse_Button;
import jpt.Event_Mouse_Scroll;
import jpt.Event_Key;

//------------------------------------------------------------------------
// Test data
//------------------------------------------------------------------------
static uint32 g_num = 0;

class Event_Test : public jpt::Event
{
private:
    uint32 m_num;

public:
    Event_Test(uint32 num) : m_num(num) {}

    uint32 GetNum() const { return m_num; }
};

//------------------------------------------------------------------------
// Global event handler
//------------------------------------------------------------------------
void GlobalEventHandler(const Event_Test& eventTest)
{
    g_num = eventTest.GetNum();
}

static bool RegisterGlobal()
{
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(&GlobalEventHandler);

    Event_Test event(123);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_num == 123);
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(handle));

    return true;
}

static bool UnregisterGlobal()
{
    // Register again and get handle
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(&GlobalEventHandler);

    // Unregister using handle
    jpt::EventManager::GetInstance().Unregister(handle);

    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(handle));

    Event_Test event(456);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_num == 456);

    return true;
}

//------------------------------------------------------------------------
// Lambda event handler
//------------------------------------------------------------------------
static bool Lambda()
{
    // Test data
    uint32 num = 0;
    auto testLambda = [&num](const Event_Test& eventTest)
        {
            num = eventTest.GetNum();
        };

    // Register - get handle
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(testLambda);
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(handle));

    Event_Test event(789);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(num == 789);

    // Unregister using handle
    jpt::EventManager::GetInstance().Unregister(handle);
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(handle));

    Event_Test event2(101112);
    jpt::EventManager::GetInstance().Send(event2);

    JPT_ENSURE(num == 789);  // Should still be 789, not 101112

    return true;
}

//------------------------------------------------------------------------
// Lambda with move-only captures
//------------------------------------------------------------------------
static bool LambdaWithMoveCapture()
{
    // Test data
    uint32 num = 0;

    // Lambda that captures by move (simulating unique_ptr or similar)
    auto testLambda = [&num, capturedValue = 999](const Event_Test& eventTest)
        {
            num = eventTest.GetNum() + capturedValue;
        };

    // Register - this will move the lambda
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(jpt::Move(testLambda));

    Event_Test event(1);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(num == 1000);  // 1 + 999

    // Unregister
    jpt::EventManager::GetInstance().Unregister(handle);

    return true;
}

//------------------------------------------------------------------------
// Member functions
//------------------------------------------------------------------------
class Listener
{
private:
    uint32 m_num = 0;

public:
    void MemberFunction(const Event_Test& eventTest)
    {
        m_num = eventTest.GetNum();
    }

    uint32 GetNum() const { return m_num; }
};

Listener g_listener;

static bool RegisterMemberFunction()
{
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(&g_listener, &Listener::MemberFunction);
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(handle));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening<Event_Test>(&g_listener));

    Event_Test event(131415);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_listener.GetNum() == 131415);

    return true;
}

static bool UnregisterMemberFunction()
{
    // Unregister by listener pointer (old API still works for member functions)
    jpt::EventManager::GetInstance().Unregister<Event_Test>(&g_listener);
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening<Event_Test>(&g_listener));

    Event_Test event(0);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_listener.GetNum() == 131415);  // Should still be 131415

    return true;
}

static bool UnregisterMemberFunctionByHandle()
{
    // Register and unregister using handle
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(&g_listener, &Listener::MemberFunction);

    Event_Test event(999);
    jpt::EventManager::GetInstance().Send(event);
    JPT_ENSURE(g_listener.GetNum() == 999);

    jpt::EventManager::GetInstance().Unregister(handle);
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(handle));

    Event_Test event2(888);
    jpt::EventManager::GetInstance().Send(event2);
    JPT_ENSURE(g_listener.GetNum() == 999);  // Should still be 999

    return true;
}

//------------------------------------------------------------------------
// Unregister all
//------------------------------------------------------------------------
static bool UnregisterAll()
{
    auto testLambda = [](const Event_Test& eventTest)
        {
            g_num = eventTest.GetNum();
        };

    jpt::EventHandle handle1 = jpt::EventManager::GetInstance().Register<Event_Test>(&GlobalEventHandler);
    jpt::EventHandle handle2 = jpt::EventManager::GetInstance().Register<Event_Test>(&g_listener, &Listener::MemberFunction);
    jpt::EventHandle handle3 = jpt::EventManager::GetInstance().Register<Event_Test>(testLambda);

    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(handle1));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(handle2));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(handle3));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening<Event_Test>(&g_listener));

    jpt::EventManager::GetInstance().UnregisterAll<Event_Test>();

    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(handle1));
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(handle2));
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(handle3));
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening<Event_Test>(&g_listener));

    return true;
}

//------------------------------------------------------------------------
// Multiple handlers for same event
//------------------------------------------------------------------------
static bool MultipleHandlers()
{
    uint32 num1 = 0;
    uint32 num2 = 0;
    uint32 num3 = 0;

    auto lambda1 = [&num1](const Event_Test& e) { num1 = e.GetNum(); };
    auto lambda2 = [&num2](const Event_Test& e) { num2 = e.GetNum() * 2; };
    auto lambda3 = [&num3](const Event_Test& e) { num3 = e.GetNum() * 3; };

    jpt::EventHandle h1 = jpt::EventManager::GetInstance().Register<Event_Test>(lambda1);
    jpt::EventHandle h2 = jpt::EventManager::GetInstance().Register<Event_Test>(lambda2);
    jpt::EventHandle h3 = jpt::EventManager::GetInstance().Register<Event_Test>(lambda3);

    Event_Test event(10);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(num1 == 10);
    JPT_ENSURE(num2 == 20);
    JPT_ENSURE(num3 == 30);

    // Unregister middle handler
    jpt::EventManager::GetInstance().Unregister(h2);

    Event_Test event2(5);
    jpt::EventManager::GetInstance().Send(event2);

    JPT_ENSURE(num1 == 5);
    JPT_ENSURE(num2 == 20);  // Should still be 20
    JPT_ENSURE(num3 == 15);

    // Clean up
    jpt::EventManager::GetInstance().Unregister(h1);
    jpt::EventManager::GetInstance().Unregister(h3);

    return true;
}

//------------------------------------------------------------------------
// Mouse button press
//------------------------------------------------------------------------
static bool RegisterEvents()
{
    jpt::EventHandle h1 = jpt::EventManager::GetInstance().Register<jpt::Event_Mouse_Button>([](const jpt::Event_Mouse_Button& eventMouseButtonPress)
        {
            JPT_LOG("Mouse button press: %s, x: %f, y: %f", jpt::ToString(eventMouseButtonPress.GetButton()).ConstBuffer(), eventMouseButtonPress.GetX(), eventMouseButtonPress.GetY());
        });

    jpt::EventHandle h2 = jpt::EventManager::GetInstance().Register<jpt::Event_Mouse_Scroll>([](const jpt::Event_Mouse_Scroll& eventMouseScroll)
        {
            JPT_LOG("Mouse Scroll: y: %f", eventMouseScroll.GetY());
        });

    jpt::EventHandle h3 = jpt::EventManager::GetInstance().Register<jpt::Event_Key>([](const jpt::Event_Key& eventKey)
        {
            JPT_LOG("Key: %s, state: %s", jpt::ToString(eventKey.GetKey()).ConstBuffer(), jpt::ToString(eventKey.GetState()).ConstBuffer());
        });

    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(h1));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(h2));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(h3));

    return true;
}

export bool RunUnitTests_EventSystem()
{
    JPT_ENSURE(g_num == 0);

    JPT_ENSURE(RegisterGlobal());
    JPT_ENSURE(UnregisterGlobal());

    JPT_ENSURE(Lambda());
    JPT_ENSURE(LambdaWithMoveCapture());

    JPT_ENSURE(RegisterMemberFunction());
    JPT_ENSURE(UnregisterMemberFunction());
    JPT_ENSURE(UnregisterMemberFunctionByHandle());

    JPT_ENSURE(UnregisterAll());
    JPT_ENSURE(MultipleHandlers());

    JPT_ENSURE(RegisterEvents());

    return true;
}