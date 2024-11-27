// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module jpt.Input.Manager;

import jpt.Input.KeyCode;
import jpt.Input.Backend;
import jpt.Input.Backend_GLFW;

import jpt.Framework.Enums;

import jpt.DynamicArray;
import jpt.String;

import jpt.Time.TypeDefs;

using namespace jpt::Input;

export namespace jpt
{
	class InputManager
	{
	private:
		Backend* m_pBackend = nullptr;		/**< Input backend framework */

	public:
		JPT_DECLARE_SINGLETON(InputManager);

		bool PreInit();
		bool Init();
		void Update(TimePrecision deltaSeconds);
		void Shutdown();

		uint32 FromKeyCode(KeyCode key) const;
		KeyCode ToKeyCode(uint32 key) const;
	};

	bool InputManager::PreInit()
	{
		const Framework_API frameworkAPI = GetApplication()->GetFrameworkAPI();
		switch (frameworkAPI.Value())
		{
		case Framework_API::GLFW:
			m_pBackend = new Backend_GLFW();
			break;

		default:
			JPT_ERROR("Unsupported framework API: " + frameworkAPI.ToString());
			return false;
		}

		m_pBackend->PreInit();
		
		return true;
	}

	bool InputManager::Init()
	{
		m_pBackend->Init();

		return true;
	}

	void InputManager::Update(TimePrecision deltaSeconds)
	{
		m_pBackend->Update(deltaSeconds);
	}

	void InputManager::Shutdown()
	{
		JPT_DELETE(m_pBackend);
	}

	uint32 InputManager::FromKeyCode(KeyCode key) const
	{
		return m_pBackend->FromKeyCode(key);
	}

	KeyCode InputManager::ToKeyCode(uint32 key) const
	{
		return m_pBackend->ToKeyCode(key);
	}
}