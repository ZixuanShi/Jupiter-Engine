// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "ApplicationLayer/Applications/ApplicationBase.h"

namespace jpt
{
	class DX12Window;

	/** Application implementation for Windows 64 */
	class JPT_API Win64Application : public ApplicationBase
	{
	private:
		using Super = ApplicationBase;

		/** Parameter data passed into created window */
		struct lpParamData
		{
			Win64Application* m_pWin64Application = nullptr;
			DX12Window* m_pDX12Window = nullptr;
		};

	private:
		/** The handle to the window(s) created by Jupiter Engine */
		static HWND m_hwnd;
		lpParamData m_lpParamData;

		/** The handle to an instance or handle to a module.
		    The operating system uses this value to identify the executable or EXE when it's loaded in memory. 
		    Certain Windows functions need the instance handle, for example to load icons or bitmaps. */
		HINSTANCE m_hInstance = nullptr;

		/** A flag that indicates whether the main application window is minimized, maximized, or shown normally. */
		int32 m_nCmdShow = 0;

	public:
		virtual bool PreInit() override;
		virtual bool Init() override;
		virtual void Update() override;

	public:
		static HWND GetHwnd() { return m_hwnd; }
		void SetHINSTANCE(HINSTANCE hInstance) { m_hInstance = hInstance; }
		void SetnCmdShow(int32 nCmdShow) { m_nCmdShow = nCmdShow; }

	protected:
		/** Overridden message handler when the Jupiter Windows have new events
			@param hWnd:		A handle to the window procedure that received the message.
			@param message:		The message received by window
			@param wParam:		Additional message information. The content of this parameter depends on the value of the Msg parameter.
			@param lParam:		Additional message information. The content of this parameter depends on the value of the Msg parameter.
			@return				LRESULT of The return value is the result of the message processing and depends on the message. */
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}