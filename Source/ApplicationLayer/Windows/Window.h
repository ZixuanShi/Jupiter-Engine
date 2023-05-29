#pragma once

namespace jpt
{
	// Base class for Windows that people see on screen
	class WindowBase
	{
	protected:
		std::wstring m_title = L"Jupiter Engine";

		// Viewport dimensions
		uint32 m_width = 1280;
		uint32 m_height = 720;
		float m_aspectRatio = 1.0f;

		// Adapter info
		bool m_useWarpDevice = false;

	public:
		virtual ~WindowBase() {}
		
		virtual bool Init() { return true; }
		virtual void Update() {};
		virtual void Render() {};
		virtual void Clean() {};

		virtual void OnKeyDown(uint8) {}
		virtual void OnKeyUp(uint8) {}

		// Returns: A pointer to a created window based on current platform
		static WindowBase* Create();

		// Parse the command arguments and store into argv[], store the count to argc
		virtual void ParseCommandLineArgs(WCHAR* argv[], int32 argc) = 0;

		const WCHAR* GetTitle() const { return m_title.c_str(); }
		uint32 GetWidth() const { return m_width; }
		uint32 GetHeight() const { return m_height; }
		float GetAspectRatio() const { return m_aspectRatio; }
		bool GetUseWarpDevice() const { return m_useWarpDevice; }
	};
}