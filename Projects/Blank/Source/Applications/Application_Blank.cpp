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

	jpt::String str = "Hello World AAA";
	JPT_LOG(str);

	str.Reserve(26);
	str.Reserve(26);
	str.Reserve(26);
	str.Reserve(26);
	str.Reserve(26);

	return true;
}

void Application_Blank::Update(jpt::TimePrecision deltaSeconds)
{
	Super::Update(deltaSeconds);

	m_foo.Update(deltaSeconds);
	m_bar.Update(deltaSeconds);
}

JPT_SYNC_CLIENT(Blank)