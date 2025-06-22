// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

#include <unordered_map>
#include <typeinfo>

module Application_Blank;

import jpt.CoreModules;

import jpt.CommandLine;
import jpt.SystemPaths;

import jpt.RawInput;
import jpt.Input_Enums;

import jpt.Window;
import jpt.WindowManager;

import jpt.Renderer;

import jpt.EventManager;
import jpt.Event_Key;

import jpt.StaticHashMap;

class Foo
{
    const int m = 0;

public:

    Foo() = default;
    Foo(int n)
    {
        *const_cast<int*>(&m) = n;
    }
};

bool Application_Blank::PreInit()
{
    JPT_ENSURE(Super::PreInit());

    Foo foo(5);

    jpt::StaticHashMap<int32, jpt::String, 5> map;

    map.Add(10, "One");
    map.Add(22, "Two");
    map.Add(-36, "Three");
    map.Add(489, "Four");
    map.Add(-55, "Five");

    //JPT_LOG(*map.Find(22));
    JPT_LOG(map);

    m_pFoo = new Entity_Foo();
    m_pFoo->PreInit();

    jpt::EventManager::GetInstance().Register<jpt::Event_Key>([]([[maybe_unused]] const jpt::Event_Key& keyPressEvent) 
        {
        });

    return true;
}

void Application_Blank::Update(TimePrecision deltaSeconds)
{
    Super::Update(deltaSeconds);

    m_pFoo->Update(deltaSeconds);
}

void Application_Blank::Shutdown()
{
    JPT_SHUTDOWN(m_pFoo);

    Super::Shutdown();
}

JPT_SYNC_CLIENT(Blank)