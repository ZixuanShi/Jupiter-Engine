// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Core/Validation/Assert.h"

export module jpt.FileEnums;

import jpt.TypeDefs;

export namespace jpt::File
{
    enum class Source : uint8
    {
        Engine,        // Engine development folder
        Client,        // Client development folder
        Output,        // Build output folder where the executable is located
        Saved,         // Output/_Saved

        Baked,         // Release: Output/Baked. Non-Release: Engine/Baked or Client/Baked
    };
}
