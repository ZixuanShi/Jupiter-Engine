#include "Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"

import jpt.CoreModules;

import jpt.CommandLine;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	return true;
}
