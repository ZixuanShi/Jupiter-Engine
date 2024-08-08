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

export namespace jpt::Input
{
	class Manager
	{
	private:
		DynamicArray<Device_Base*> m_devices;	/**< Array of all connected input devices */
		Backend_Base* m_pBackend = nullptr;		/**< Input backend framework */

	public:
		bool PreInit();
		bool Init();
		void Update();
		void Terminate();

		bool IsKeyPressed(KeyCode key) const;
	};

	bool Manager::PreInit()
	{
		ProjectSettings& projectSettings = ProjectSettings::GetInstance();
		Framework::API frameworkAPI = projectSettings.Get<String>("framework_api");
		switch (frameworkAPI.Value())
		{
		case Framework::API::GLFW:
			m_pBackend = new Backend_GLFW();
			break;

		default:
			JPT_ERROR("Unsupported framework API: " + frameworkAPI.ToString());
			return false;
		}
		
		return true;
	}

	bool Manager::Init()
	{
		m_pBackend->Init();

		// Initialize input devices

		return true;
	}

	void Manager::Update()
	{
		m_pBackend->Update();

		for (Device_Base* pDevice : m_devices)
		{
			pDevice->Update();
		}
	}

	void Manager::Terminate()
	{
		JPT_TERMINATE(m_pBackend);

		for (Device_Base* pDevice : m_devices)
		{
			pDevice->Terminate();
		}
	}

	bool Manager::IsKeyPressed(KeyCode key) const
	{
		return m_pBackend->IsKeyPressed(key);
	}
}