// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

import jpt.CoreModules;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	m_foo.PreInit();
	m_bar.PreInit();

	// 1. String, small
	// 2. WString, small
	// 3. String, large
	// 4. WString, large

	jpt::String smallStr = "Hello World AAA";
	jpt::WString smallWStr = L"Hello World AAA";
	jpt::String largeStr = "Hello World Jupiter Engine";
	jpt::WString largeWStr = L"Hello World Jupiter Engine";

	JPT_LOG(smallStr);
	JPT_LOG(smallWStr);
	JPT_LOG(largeStr);
	JPT_LOG(largeWStr);

	return true;
}

void Application_Blank::Update(jpt::TimePrecision deltaSeconds)
{
	Super::Update(deltaSeconds);

	m_foo.Update(deltaSeconds);
	m_bar.Update(deltaSeconds);
}

JPT_SYNC_CLIENT(Blank)