// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

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
    jpt::EventManager::GetInstance().Register<Event_Test>(&GlobalEventHandler);

    Event_Test event(123);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_num == 123);

    return true;
}

static bool UnregisterGlobal()
{
    jpt::EventManager::GetInstance().Unregister<Event_Test>(&GlobalEventHandler);

    Event_Test event(456);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_num == 123);

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

    // Register
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(testLambda);

    Event_Test event(789);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(num == 789);

    // Unregister
    //jpt::EventManager::GetInstance().Unregister<Event_Test>(&testLambda); // This is wrong, cannot unregister lambda this way
    jpt::EventManager::GetInstance().Unregister(handle);
    Event_Test event2(101112);
    jpt::EventManager::GetInstance().Send(event2);

    JPT_ENSURE(num == 789);

    return true;
}

//------------------------------------------------------------------------
// Member functions
//------------------------------------------------------------------------
class Listener
{
private:
    uint32 m_num;

public:
    void MemberFunction(const Event_Test& eventTest)
    {
        m_num = eventTest.GetNum();
    }

    uint32 GetNum() const { return m_num; }
};

Listener g_listener;
jpt::EventHandle g_memberFunctionHandle;
static bool RegisterMemberFunction()
{
    g_memberFunctionHandle = jpt::EventManager::GetInstance().Register<Event_Test>(&g_listener, &Listener::MemberFunction);

    Event_Test event(131415);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_listener.GetNum() == 131415);

    return true;
}

static bool UnregisterMemberFunction()
{
    jpt::EventManager::GetInstance().Unregister(g_memberFunctionHandle);

    Event_Test event(0);
    jpt::EventManager::GetInstance().Send(event);

    JPT_ENSURE(g_listener.GetNum() == 131415);

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
    jpt::EventManager::GetInstance().Register<Event_Test>(&GlobalEventHandler);
    g_memberFunctionHandle = jpt::EventManager::GetInstance().Register<Event_Test>(&g_listener, &Listener::MemberFunction);
    jpt::EventHandle handle = jpt::EventManager::GetInstance().Register<Event_Test>(testLambda);

    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening<Event_Test>(&GlobalEventHandler));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(g_memberFunctionHandle));
    JPT_ENSURE(jpt::EventManager::GetInstance().IsListening(handle));

    jpt::EventManager::GetInstance().UnregisterAll<Event_Test>();

    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening<Event_Test>(&GlobalEventHandler));
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(g_memberFunctionHandle));
    JPT_ENSURE(!jpt::EventManager::GetInstance().IsListening(handle));

    return true;
}

//------------------------------------------------------------------------
// Mouse button press
//------------------------------------------------------------------------
static bool RegisterEvents()
{
    jpt::EventManager::GetInstance().Register<jpt::Event_Mouse_Button>([](const jpt::Event_Mouse_Button& eventMouseButtonPress)
        {
            const char* stateStr  = jpt::ToString(eventMouseButtonPress.GetState()).ConstBuffer();
            const char* buttonStr = jpt::ToString(eventMouseButtonPress.GetButton()).ConstBuffer();
            JPT_LOG("Mouse Button %s: %s, x: %f, y: %f", stateStr, buttonStr, eventMouseButtonPress.GetX(), eventMouseButtonPress.GetY());
        });

    jpt::EventManager::GetInstance().Register<jpt::Event_Mouse_Scroll>([](const jpt::Event_Mouse_Scroll& eventMouseScroll)
        {
            JPT_LOG("Mouse Scroll: y: %f", eventMouseScroll.GetY());
        });

    jpt::EventManager::GetInstance().Register<jpt::Event_Key>([](const jpt::Event_Key& eventKey)
        {
            const char* stateStr = jpt::ToString(eventKey.GetState()).ConstBuffer();
            const char* keyStr   = jpt::ToString(eventKey.GetKey()).ConstBuffer();
            JPT_LOG("Key %s: %s", stateStr, keyStr);
        });

    return true;
}

export bool RunUnitTests_EventSystem()
{
    JPT_ENSURE(g_num == 0);

    JPT_ENSURE(RegisterGlobal());
    JPT_ENSURE(UnregisterGlobal());

    JPT_ENSURE(Lambda());

    JPT_ENSURE(RegisterMemberFunction());
    JPT_ENSURE(UnregisterMemberFunction());

    JPT_ENSURE(UnregisterAll());

    JPT_ENSURE(RegisterEvents());

    return true;
}