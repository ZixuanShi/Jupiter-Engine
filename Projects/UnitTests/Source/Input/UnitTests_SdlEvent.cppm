// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// Interface only: the body reaches GetApp(), whose header carries `import jpt.ApplicationBase`.
// Including that here would be a cycle in the module import graph, but an *implementation* unit's
// global module fragment may carry it -- which is what UnitTests_SdlEvent.cpp does, the same shape
// as Source/Graphics/RendererBase.cpp.

export module UnitTests_SdlEvent;

import jpt.TestFramework;

export void RunUnitTests_SdlEventTranslation(jpt::TestCase& test);
