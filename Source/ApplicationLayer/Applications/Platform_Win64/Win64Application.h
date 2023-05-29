#pragma once

#include "ApplicationLayer/Applications/ApplicationBase.h"

namespace jpt
{
	// Application implementation for Windows 64
	class JPT_API Win64Application : public ApplicationBase
	{
	private:
		using Super = ApplicationBase;

	private:
		static HWND m_hwnd;

		HINSTANCE m_hInstance = nullptr;
		int32 m_nCmdShow = 0;

	public:
		virtual bool PreInit() override;
		virtual bool Init() override;
		virtual void RunGameLoop() override;

		static HWND GetHwnd() { return m_hwnd; }
		void SetHINSTANCE(HINSTANCE hInstance) { m_hInstance = hInstance; }
		void SetnCmdShow(int32 nCmdShow) { m_nCmdShow = nCmdShow; }

	protected:
		// Message handler
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}