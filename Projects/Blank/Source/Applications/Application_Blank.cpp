// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"

import jpt.CoreModules;

import jpt.Input.Manager;
import jpt.Input.KeyCode;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

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

#pragma region Engine-Client Communications
constexpr const wchar_t* jpt::File::GetClientDirW() { return JPT_CLIENT_DIR_W; }
constexpr const wchar_t* jpt::File::GetOutputDirW() { return JPT_OUTPUT_DIR_W; }
#pragma endregion