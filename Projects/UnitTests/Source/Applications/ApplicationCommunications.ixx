// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file overrides the global communication functions through out both engine and client

export module ApplicationCommunications;

import jpt.File.Path.Helpers;

/** Must Overrides GetClientDir here */
const char* jpt::File::GetClientDir()
{
	return JPT_CLIENT_DIR;
}

const wchar_t* jpt::File::GetClientDirW()
{
	return JPT_CLIENT_DIR_W;
}
