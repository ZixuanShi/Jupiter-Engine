// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Framework_Create;

import jpt.Framework;
import jpt.Framework_Enums;

export namespace jpt
{
    FrameworkAPI FindFrameworkAPI();
    Framework* Framework_Create(FrameworkAPI api);
}