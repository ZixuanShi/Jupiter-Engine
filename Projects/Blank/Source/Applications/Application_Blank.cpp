// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

import jpt.CoreModules;

import jpt.InputManager;
import jpt.Input.KeyCode;

import jpt.Event.Manager;
import jpt.Event.Window.Close;
import jpt.Event.Keyboard.KeyPress;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	m_foo.PreInit();
	m_bar.PreInit();

	jpt::EventManager::GetInstance().Register<jpt::Event_Keyboard_KeyPress>([this](const jpt::Event_Keyboard_KeyPress& eventKeyboardKeyPress)
		{
			if (eventKeyboardKeyPress.GetKey() == jpt::Input::KeyCode::Keyboard_Escape)
			{
				m_shouldShutdown = true;
			}
		});

	return true;
}

void Application_Blank::Update(jpt::TimePrecision deltaSeconds)
{
	Super::Update(deltaSeconds);

	m_foo.Update(deltaSeconds);
	m_bar.Update(deltaSeconds);
}

JPT_SYNC_CLIENT(Blank)