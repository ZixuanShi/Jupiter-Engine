// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Application_JupiterUnitTests.h"

#include "Core/Minimal/CoreHeaders.h"
#include "Applications/SyncClient.h"

import jpt.CommandLine;
import jpt.Utilities;
import jpt.File.Path.Helpers;

import UnitTests_Applications;
import UnitTests_Core;
import UnitTests_Data;
import UnitTests_Debugging;
import UnitTests_System;
import UnitTests_Scratch;

bool Application_JupiterUnitTests::PreInit()
{
	//jpt::CommandLine::GetInstance().Set("no_window");

	JPT_ENSURE(Super::PreInit());

	return true;
}

bool Application_JupiterUnitTests::Init()
{
	JPT_ENSURE(Super::Init());

	JPT_LOG("Applications Unit Tests %s", RunUnitTests_Applications() ? "Succeeded" : "Failed");
	JPT_LOG("Core         Unit Tests %s", RunUnitTests_Core()         ? "Succeeded" : "Failed");
	JPT_LOG("Data         Unit Tests %s", RunUnitTests_Data()         ? "Succeeded" : "Failed");
	JPT_LOG("Debugging    Unit Tests %s", RunUnitTests_Debugging()    ? "Succeeded" : "Failed");
	JPT_LOG("System       Unit Tests %s", RunUnitTests_System()       ? "Succeeded" : "Failed");
	JPT_LOG("Scratch      Unit Tests %s", RunUnitTests_Scratch()      ? "Succeeded" : "Failed");

	return true;
}

JPT_SYNC_CLIENT(Application_JupiterUnitTests)