// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.FilePathUtils;

import jpt.File_Enums;
import jpt.FilePath;

export namespace jpt::File
{
    Path Combine(Source source, const Path& relativePath);

    /** @return Either full absolute path if ran from VS debugger, or relative path if ran from executable
        @param relativePath        Expected in client's folder. Not engine */
    Path FixDependencies(const Path& relativePath);
}