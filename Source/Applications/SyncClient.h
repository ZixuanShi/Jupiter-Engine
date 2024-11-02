// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Should be called in client's Application.cpp file once
#define JPT_SYNC_CLIENT(ApplicationName)                                             \
jpt::Application* jpt::GetApplication()                                              \
{																					 \
	static ApplicationName app;													     \
	return &app;																	 \
}                                                                                    \
																					 \
constexpr const wchar_t* jpt::File::GetClientDirW() { return JPT_CLIENT_DIR_W; }     \
constexpr const wchar_t* jpt::File::GetOutputDirW() { return JPT_OUTPUT_DIR_W; }     

