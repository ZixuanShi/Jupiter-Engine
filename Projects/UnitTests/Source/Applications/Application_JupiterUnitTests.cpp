﻿// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "System/Environment/SyncClient.h"

module Application_JupiterUnitTests;

import jpt.CoreModules;
import jpt.CommandLine;
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

    JPT_LOG("Applications Unit Tests %s", RunUnitTests_Applications() ? "Succeeded" : "Failed");
    JPT_LOG("Core         Unit Tests %s", RunUnitTests_Core()         ? "Succeeded" : "Failed");
    JPT_LOG("Data         Unit Tests %s", RunUnitTests_Data()         ? "Succeeded" : "Failed");
    JPT_LOG("Debugging    Unit Tests %s", RunUnitTests_Debugging()    ? "Succeeded" : "Failed");
    JPT_LOG("Frameworks   Unit Tests %s", RunUnitTests_Frameworks()   ? "Succeeded" : "Failed");
    JPT_LOG("System       Unit Tests %s", RunUnitTests_System()       ? "Succeeded" : "Failed");
    JPT_LOG("Scratch      Unit Tests %s", RunUnitTests_Scratch()      ? "Succeeded" : "Failed");

    return true;
}

JPT_SYNC_CLIENT(JupiterUnitTests)