// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module jpt.Input.Manager;

import jpt.Input.Device;
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
		DynamicArray<Device*> m_devices;	/**< Array of all connected input devices */
		Backend* m_pBackend = nullptr;		/**< Input backend framework */

	public:
		JPT_DECLARE_SINGLETON(InputManager);

		bool PreInit();
		bool Init();
		void Update(TimePrecision deltaSeconds);
		void Shutdown();

		/** @return		true if the key is currently down at this frame */
		bool IsDown(KeyCode key) const;

		/** @return		true if the key is currently up at this frame */
		bool IsUp(KeyCode key) const;

		/** @return		true if the key(s) was newly pressed at this frame. Last frame was Up */
		bool IsPressed(KeyCode key) const;
		bool ArePressed(const DynamicArray<KeyCode>& keys) const;

		/** @return		true if the key was newly released at this frame. Last frame was Down */
		bool IsReleased(KeyCode key) const;

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

		// Add connected devices

		for (Device* pDevice : m_devices)
		{
			if (!pDevice->PreInit())
			{
				return false;
			}
		}
		
		return true;
	}

	bool InputManager::Init()
	{
		m_pBackend->Init();

		for (Device* pDevice : m_devices)
		{
			if (!pDevice->Init())
			{
				return false;
			}
		}

		return true;
	}

	void InputManager::Update(TimePrecision deltaSeconds)
	{
		for (Device* pDevice : m_devices)
		{
			pDevice->Update(deltaSeconds);
		}
	}

	void InputManager::Shutdown()
	{
		JPT_DELETE(m_pBackend);

		for (Device* pDevice : m_devices)
		{
			pDevice->Shutdown();
		}
	}

	bool InputManager::IsDown(KeyCode key) const
	{
		JPT_IGNORE(key);
		return false;
	}

	bool InputManager::IsUp(KeyCode key) const
	{
		JPT_IGNORE(key);
		return false;
	}

	bool InputManager::IsPressed(KeyCode key) const
	{
		return m_pBackend->IsPressed(key);
	}

	bool InputManager::ArePressed(const DynamicArray<KeyCode>& keys) const
	{
		for (KeyCode key : keys)
		{
			if (!m_pBackend->IsPressed(key))
			{
				return false;
			}
		}

		return true;
	}

	bool InputManager::IsReleased(KeyCode key) const
	{
		return m_pBackend->IsReleased(key);
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