// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Base class for Windows that people see on screen */
	class WindowBase
	{
	protected:
		jpt::wstring m_title = L"Jupiter Engine";

		// Viewport dimensions
		uint32 m_width      = 1280;
		uint32 m_height     = 720;
		float m_aspectRatio = 1.0f;

		// Adapter info
		bool m_useWarpDevice = false;

	public:
		virtual ~WindowBase() = default;

		virtual bool Init();
		virtual void Update() {};
		virtual void Render() {};
		virtual void Clean() {};

	public:
		virtual void OnKeyDown(uint8) {}
		virtual void OnKeyUp(uint8) {}

		/** @return  A pointer to a created window based on current platform */
		static WindowBase* Create();

		/** Parse the command arguments and store into argv[], store the count to argc 
			@param argv[]:		The command arguments passed in to this window
			@param argc:		The count of arguments passed in to this window*/
		virtual void ParseCommandLineArgs(wchar_t* argv[], int32 argc) = 0;

	public:
		const wchar_t* GetTitle() const { return m_title.c_str(); }
		uint32 GetWidth() const { return m_width; }
		uint32 GetHeight() const { return m_height; }
		float GetAspectRatio() const { return m_aspectRatio; }
		bool GetUseWarpDevice() const { return m_useWarpDevice; }

		void SetTitle(const wchar_t* title) { m_title = title; }
		void SetWidth(uint32 width) { m_width = width; }
		void SetHeight(uint32 height) { m_height = height; }
		void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
		void SetUseWarpDevice(bool useWarpDevice) { m_useWarpDevice = useWarpDevice; }
	};
}