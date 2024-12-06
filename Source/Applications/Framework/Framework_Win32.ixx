// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module jpt.Framework_Win32;

import jpt.Framework;

import jpt.Window;
import jpt.Window.Manager;
import jpt.Window_Win32;

import jpt.Platform.Win64;

namespace jpt
{
	export class Framework_Win32 final : public Framework
	{
		using Super = Framework;

	private:
		WNDCLASSEX m_windowClass = { 0 };

	public:
		virtual bool PreInit() override;
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

	public:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		Platform_Win64* GetPlatformWin64() const;
		const WNDCLASSEX& GetWindowClass() const { return m_windowClass; }
	};

	bool Framework_Win32::PreInit()
	{
		JPT_ENSURE(Super::PreInit());

		// Initialize the window class.
		m_windowClass.cbSize = sizeof(WNDCLASSEX);
		m_windowClass.style = CS_HREDRAW | CS_VREDRAW;
		m_windowClass.lpfnWndProc = &Framework_Win32::WindowProc;
		m_windowClass.hInstance = GetPlatformWin64()->GetHINSTANCE();
		m_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		m_windowClass.lpszClassName = Window_Win32::kClassName;
		RegisterClassEx(&m_windowClass);

		return true;
	}

	bool Framework_Win32::Init()
	{
		JPT_ENSURE(Super::Init());

		return true;
	}

	void Framework_Win32::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		MSG msg = { };
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Framework_Win32::Shutdown()
	{
		UnregisterClass(m_windowClass.lpszClassName, GetPlatformWin64()->GetHINSTANCE());

		Super::Shutdown();
	}

	LRESULT Framework_Win32::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Window_Win32* pWindow = reinterpret_cast<Window_Win32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		Application* pApp = GetApplication();

		switch (message)
		{
			case WM_CREATE:
			{
				LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));

				return 0;
			}
			case WM_DESTROY:
			{
				pApp->GetWindowManager()->Destroy(pWindow);

				if (pApp->GetMainWindow() == pWindow)
				{
					PostQuitMessage(0);
				}

				return 0;
			}
			default:
			{ 
				return DefWindowProc(hWnd, message, wParam, lParam); 
			}
		}
	}

	Platform_Win64* Framework_Win32::GetPlatformWin64() const
	{
		Platform_Win64* pPlatformWin64 = static_cast<Platform_Win64*>(GetApplication()->GetPlatform());
		JPT_ASSERT(pPlatformWin64, "Platform is not set to win64");
		return pPlatformWin64;
	}
}