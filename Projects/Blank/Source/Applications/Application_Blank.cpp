// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

#include <unordered_map>

import jpt.CoreModules;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	m_foo.PreInit();
	m_bar.PreInit();

	std::unordered_map<int, int> map;
	JPT_LOG(map[1]);
	map.insert({ 1, 2 });
	map.insert({ 1, 3 });
	JPT_LOG(map[1]);

	jpt::HashMap<int, int> jptMap;
	JPT_LOG(jptMap[1]);
	jptMap.Add(1, 2);
	jptMap.Add(1, 3);
	JPT_LOG(jptMap[1]);

	return true;
}

void Application_Blank::Update(jpt::TimePrecision deltaSeconds)
{
	Super::Update(deltaSeconds);

	m_foo.Update(deltaSeconds);
	m_bar.Update(deltaSeconds);
}

JPT_SYNC_CLIENT(Blank)