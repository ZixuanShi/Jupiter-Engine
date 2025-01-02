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

import jpt.Event.Manager;
import jpt.Event.Window.Close;
import jpt.Event.Keyboard.KeyPress;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	m_pFoo = new Entity_Foo();
	m_pFoo->PreInit();

	jpt::EventManager::GetInstance().Register<jpt::Event_Keyboard_KeyPress>([this](const jpt::Event_Keyboard_KeyPress& eventKeyboardKeyPress)
		{
			if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::Escape)
			{
				m_shouldShutdown = true;
			}
			if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::N)
			{
				jpt::Window* pWindow = m_pWindowManager->Create();
				GetRenderer()->RegisterWindow(pWindow);
			}
		});

	JPT_LOG(jpt::System::Paths::GetInstance().GetOutputDir());
	JPT_LOG(jpt::System::Paths::GetInstance().GetExecutablePath());

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