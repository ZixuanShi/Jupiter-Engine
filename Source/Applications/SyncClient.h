// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Should be called in client's Application.cpp file once
#define JPT_SYNC_CLIENT(ApplicationName)                                             \
jpt::Application* jpt::GetApplication()                                              \
{																					 \
	static Application_##ApplicationName app;									     \
	return &app;																	 \
}                                                                                    \
																					 \
const char* jpt::Application::GetName() const { return #ApplicationName; }           \
                                                                                     \
constexpr const wchar_t* jpt::File::GetClientDirW() { return JPT_CLIENT_DIR_W; }     \
constexpr const wchar_t* jpt::File::GetOutputDirW() { return JPT_OUTPUT_DIR_W; }     

