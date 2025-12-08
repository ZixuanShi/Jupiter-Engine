// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "Applications/Utils/SyncClient.h"

module Application_JupiterUnitTests;

import jpt.CoreModules;
import jpt.LaunchArgs;
import jpt.Utilities;
import jpt.FilePathHelpers;

import UnitTests_Applications;
import UnitTests_Core;
import UnitTests_Data;
import UnitTests_Debugging;
import UnitTests_Frameworks;
import UnitTests_System;
import UnitTests_Scratch;

bool Application_JupiterUnitTests::PreInit()
{
    JPT_ENSURE(Super::PreInit());

    return true;
}

bool Application_JupiterUnitTests::Init()
{
    JPT_ENSURE(Super::Init());

    JPT_INFO("Applications Unit Tests %s", RunUnitTests_Applications() ? "Succeeded" : "Failed");
    JPT_INFO("Core         Unit Tests %s", RunUnitTests_Core()         ? "Succeeded" : "Failed");
    JPT_INFO("Data         Unit Tests %s", RunUnitTests_Data()         ? "Succeeded" : "Failed");
    JPT_INFO("Debugging    Unit Tests %s", RunUnitTests_Debugging()    ? "Succeeded" : "Failed");
    JPT_INFO("Frameworks   Unit Tests %s", RunUnitTests_Frameworks()   ? "Succeeded" : "Failed");
    JPT_INFO("System       Unit Tests %s", RunUnitTests_System()       ? "Succeeded" : "Failed");
    JPT_INFO("Scratch      Unit Tests %s", RunUnitTests_Scratch()      ? "Succeeded" : "Failed");

    return true;
}

JPT_SYNC_CLIENT(JupiterUnitTests)