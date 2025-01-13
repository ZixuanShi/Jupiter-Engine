// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

import jpt.CoreModules;

import jpt.CommandLine;
import jpt.System.Paths;

import jpt.RawInput;
import jpt.Input.KeyCode;

import jpt.Window;
import jpt.Window.Manager;

import jpt.Renderer;

import jpt.Object.Pool;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	m_pFoo = new Entity_Foo();
	m_pFoo->PreInit();

	jpt::ObjectPool<jpt::String> stringPool;

	stringPool.Init(8);

	auto handle = stringPool.Acquire();
	(*handle.first) = "Hello, World!";

	JPT_LOG(*handle.first);
	JPT_LOG(handle.second);

	stringPool.Release(handle.second);

	stringPool.Shutdown();

	return true;
}

void Application_Blank::Update(jpt::TimePrecision deltaSeconds)
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