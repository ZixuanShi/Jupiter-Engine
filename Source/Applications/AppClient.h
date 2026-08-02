// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Application;

namespace jpt
{
    /** Defined by the client that owns the concrete Application, so the engine never names
        the derived type. Declared here rather than in jpt.Application because clang attaches
        an exported function to its module and would then require the definition to live
        there too. Becomes JPT_SYNC_CLIENT once Projects/<Name> exists. */
    Application& GetApplication();
}
