// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file overrides the global communication functions through out both engine and client

export module ApplicationCommunications;

import jpt.Application.Base;
import jpt.File.Path.Utils;

import UnitTestsApplication;

/** Must Overrides Application GetInstance here */
jpt::Application_Base& jpt::Application_Base::GetInstance()
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
