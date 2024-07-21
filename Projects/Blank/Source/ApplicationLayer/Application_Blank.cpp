#include "Application_Blank.h"

#include "Core/Minimal/CoreHeaders.h"

import jpt.CoreModules;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	return true;
}
