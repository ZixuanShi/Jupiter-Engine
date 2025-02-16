// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

module Application_Blank;

import jpt.CoreModules;

import jpt.CommandLine;
import jpt.System.Paths;

import jpt.RawInput;
import jpt.Input.Enums;

import jpt.Window;
import jpt.Window.Manager;

import jpt.Renderer;

import jpt.Event.Manager;
import jpt.Event.Key;

import jpt.Deque;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	m_pFoo = new Entity_Foo();
	m_pFoo->PreInit();

	jpt::EventManager::GetInstance().Register<jpt::Event_Key>([]([[maybe_unused]] const jpt::Event_Key& keyPressEvent) 
		{
		});

	//jpt::Deque<jpt::String> deque;

	//deque.PushFront("Hello");
	//deque.PushBack(" ");
	//deque.PushBack("World");

	//for (size_t i = 0; i < deque.Count(); ++i)
	//{
	//	jpt::String& str = deque[i];
	//	JPT_LOG(str);
	//}

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