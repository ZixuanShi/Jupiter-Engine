// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

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
                                                                                     \
	File::Path ClientPaths::GetClientDirW()                                          \
    { 																				 \
	    static File::Path path = JPT_CLIENT_DIR_W;                                   \
	    return path;                                                                 \
    }                                                                                \
                                                                                     \
    File::Path ClientPaths::GetOutputDirW()                                          \
    { 																				 \
	    static File::Path path = JPT_OUTPUT_DIR_W;                                   \
	    return path;                                                                 \
	}                                                                                \
}

