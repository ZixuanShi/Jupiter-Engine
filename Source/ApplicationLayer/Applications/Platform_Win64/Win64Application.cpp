// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "Win64Application.h"

#include "ApplicationLayer/Windows/DirectX/DX12Window.h"
#include "Rendering/DirectX/DX12Renderer.h"

namespace jpt
{
	HWND Win64Application::m_hwnd = nullptr;

	bool Win64Application::PreInit()
	{
		JPT_RETURN_FALSE_IF_LOG(!Super::PreInit(), "Failed Super::PreInit()");

		// Parse the command line parameters
		int32 argc;
		LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
		m_pWindow->ParseCommandLineArgs(argv, argc);
		LocalFree(argv);

		// Initialize the Window class
		WNDCLASSEX windowClass = { 0 };
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = m_hInstance;
		windowClass.hCursor = LoadCursor(nullptr, IDC_HAND);
		windowClass.lpszClassName = L"Jupiter Win64 DX12 Class";
		RegisterClassEx(&windowClass);

		RECT windowRect = { 0,0, static_cast<LONG>(m_pWindow->GetWidth()), static_cast<LONG>(m_pWindow->GetHeight()) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		// Create the window and store a handle to it
		m_hwnd = CreateWindow(
			windowClass.lpszClassName,
			m_pWindow->GetTitle(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr,        // We have no parent window.
			nullptr,        // We aren't using menus.
			m_hInstance,
			m_pWindow);

		return true;
	}

	bool Win64Application::Init()
	{
		JPT_RETURN_FALSE_IF_LOG(!Super::Init(), "Failed Super::Init()");

		ShowWindow(m_hwnd, m_nCmdShow);

		return true;
	}

	void Win64Application::RunGameLoop()
	{
		MSG msg = {};

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	LRESULT CALLBACK Win64Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		DX12Window* pDX12Window = reinterpret_cast<DX12Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (message)
		{
		case WM_CREATE:
		{
			// Save the DX12Window* passed in to CreateWindow
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		}
		return 0;

		case WM_KEYDOWN:
			if (pDX12Window)
			{
				pDX12Window->OnKeyDown(static_cast<uint8>(wParam));
			}
			return 0;

		case WM_KEYUP:
			if (pDX12Window)
			{
				pDX12Window->OnKeyUp(static_cast<uint8>(wParam));
			}
			return 0;

		case WM_PAINT:
			if (pDX12Window)
			{
				pDX12Window->Update();
				pDX12Window->Render();
			}
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			// Handle any messages the switch statement didn't.
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}