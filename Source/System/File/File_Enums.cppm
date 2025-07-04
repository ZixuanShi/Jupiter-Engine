// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Core/Validation/Assert.h"

export module jpt.File_Enums;

import jpt.TypeDefs;

export namespace jpt::File
{
    enum class Source : uint8
    {
        Engine,        // Engine development folder
        Client,        // Client development folder
        Output,        // Build output folder. This could be anywhere if Release build
        Saved,         // Output/_Saved

        Baked,         // Release: Output/Baked. Non-Release: Engine/Baked or Client/Baked
    };
}
