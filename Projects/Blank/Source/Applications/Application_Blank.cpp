// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

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

bool Application_Blank::PreInit()
{
    JPT_ENSURE(Super::PreInit());

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