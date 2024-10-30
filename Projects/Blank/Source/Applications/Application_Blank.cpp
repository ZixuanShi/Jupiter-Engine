// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"
#include "Applications/SyncClient.h"

import jpt.CoreModules;

import jpt.Input.Manager;
import jpt.Input.KeyCode;

import jpt.Status;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	jpt::Status status = jpt::Status::Pending;
	JPT_LOG(status);
	JPT_LOG(status.Value());

	for (auto itr = status.begin(); itr != status.end(); ++itr)
	{
		JPT_LOG(*itr);
	}
	
	return true;
}

void Application_Blank::Update(jpt::TimePrecision deltaSeconds)
{
	Super::Update(deltaSeconds);

	using namespace jpt::Input;

	if (jpt::Input::Manager::GetInstance().ArePressed({ KeyCode::Keyboard_A,
		                                                KeyCode::Keyboard_S,
		                                                KeyCode::Keyboard_D,
		                                                KeyCode::Keyboard_F,
		                                                KeyCode::Keyboard_Numpad_0 }))
	{
		m_shouldShutdown = true;
	}
}

JPT_SYNC_CLIENT(Application_Blank)