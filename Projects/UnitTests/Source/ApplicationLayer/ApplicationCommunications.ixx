// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file overrides the global communication functions through out both engine and client

export module ApplicationCommunications;

import jpt.ApplicationBase;
import jpt.File.PathUtils;

import UnitTestsApplication;

/** Must Overrides Application GetInstance here */
jpt::ApplicationBase& jpt::ApplicationBase::GetInstance()
{
	static JupiterUnitTestsApplication s_instance;
	return s_instance;
}

/** Must Overrides GetClientDir here */
const char* jpt::GetClientDir()
{
	return JPT_CLIENT_DIR;
}

const wchar_t* jpt::GetClientDirW()
{
	return JPT_CLIENT_DIR_W;
}
