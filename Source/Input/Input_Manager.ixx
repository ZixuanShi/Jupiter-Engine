// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module jpt.Input.Manager;

import jpt.Input.Device_Base;
import jpt.Input.KeyCode;

import jpt.Input.Backend_Base;
import jpt.Input.Backend_GLFW;

import jpt.Framework_Enums;
import jpt.DynamicArray;
import jpt.String;
import jpt.ProjectSettings;
import jpt.Time.TypeDefs;

export namespace jpt::Input
{
	class Manager
	{
	private:
		DynamicArray<Device_Base*> m_devices;	/**< Array of all connected input devices */
		Backend_Base* m_pBackend = nullptr;		/**< Input backend framework */

	public:
		JPT_SINGLETON_DECLARATION(Manager);

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
	};

	bool Manager::PreInit()
	{
		const ProjectSettings& projectSettings = ProjectSettings::GetInstance();
		const Framework::API frameworkAPI = projectSettings.Get<String>("framework_api");
		switch (frameworkAPI.Value())
		{
		case Framework::API::GLFW:
			m_pBackend = new Backend_GLFW();
			break;

		default:
			JPT_ERROR("Unsupported framework API: " + frameworkAPI.ToString());
			return false;
		}

		// Add connected devices

		for (Device_Base* pDevice : m_devices)
		{
			if (!pDevice->PreInit())
			{
				return false;
			}
		}
		
		return true;
	}

	bool Manager::Init()
	{
		for (Device_Base* pDevice : m_devices)
		{
			if (!pDevice->Init())
			{
				return false;
			}
		}

		return true;
	}

	void Manager::Update(TimePrecision deltaSeconds)
	{
		for (Device_Base* pDevice : m_devices)
		{
			pDevice->Update(deltaSeconds);
		}
	}

	void Manager::Shutdown()
	{
		JPT_DELETE(m_pBackend);

		for (Device_Base* pDevice : m_devices)
		{
			pDevice->Shutdown();
		}
	}

	bool Manager::IsDown(KeyCode key) const
	{
		JPT_IGNORE(key);
		return false;
	}

	bool Manager::IsUp(KeyCode key) const
	{
		JPT_IGNORE(key);
		return false;
	}

	bool Manager::IsPressed(KeyCode key) const
	{
		return m_pBackend->IsPressed(key);
	}

	bool Manager::ArePressed(const DynamicArray<KeyCode>& keys) const
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

	bool Manager::IsReleased(KeyCode key) const
	{
		return m_pBackend->IsReleased(key);
	}
}