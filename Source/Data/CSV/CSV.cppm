// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.CSV;

import jpt.Optional;

import jpt.FilePath;
import jpt.CSVData;

export namespace jpt
{
    Optional<CSVData> ReadCSV(const File::Path& path);
    void WriteCSV(const File::Path & path, const CSVData& data);
}