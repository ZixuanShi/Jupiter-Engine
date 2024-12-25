// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

import jpt.CoreModules;

import jpt.RawInput;
import jpt.Input.KeyCode;

import jpt.Window;
import jpt.Window.Manager;

import jpt.Renderer;

import jpt.Event.Manager;
import jpt.Event.Window.Close;
import jpt.Event.Keyboard.KeyPress;

import jpt.Entity.Component;

class EntityComponent_1 : public jpt::EntityComponent
{
};

class UpdatableEntityComponent_1 : public jpt::UpdatableEntityComponent
{
private:
	int m_data = 0;

public:
	virtual bool Init() override
	{
		m_data = jpt::RNG::Global().RangedInt(0, 100);
		return true;
	}

	virtual void Update(jpt::TimePrecision) override
	{
		JPT_LOG(m_data);
	}
};

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	m_foo.PreInit();
	m_bar.PreInit();

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

			if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::A)
			{
				m_foo.AddComponent<UpdatableEntityComponent_1>();
			}
			if (eventKeyboardKeyPress.GetKey() == jpt::Input::Key::B)
			{
				m_foo.EraseComponent<UpdatableEntityComponent_1>();
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