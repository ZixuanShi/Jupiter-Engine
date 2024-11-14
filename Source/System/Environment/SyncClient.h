// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	class ClientPaths
	{
	public:
		static const wchar_t* GetClientDirW();
		static const wchar_t* GetOutputDirW();
	};
}

// Should be called in client's Application.cpp file once
#define JPT_SYNC_CLIENT(ApplicationName)                                             \
namespace jpt                                                                        \
{                                                                                    \
	Application* GetApplication()                                                    \
	{																				 \
		static Application_##ApplicationName app;									 \
		return &app;																 \
	}                                                                                \
																					 \
	const char* Application::GetName() const { return #ApplicationName; }            \
	const wchar_t* ClientPaths::GetClientDirW() { return JPT_CLIENT_DIR_W; }         \
    const wchar_t* ClientPaths::GetOutputDirW() { return JPT_OUTPUT_DIR_W; }		 \
}

