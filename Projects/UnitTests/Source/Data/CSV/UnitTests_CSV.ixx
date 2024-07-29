// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_CSV;

import jpt.CSV;
import jpt.CSV.Data;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.File.Path;
import jpt.Optional;

const jpt::File::Path kCSVPath = { jpt::File::ESource::Client, "Assets/Test.csv" };

static bool Read()
{
    jpt::Optional<jpt::CSVData> csvData = jpt::ReadCSV(kCSVPath);
    JPT_ENSURE(csvData);

    jpt::CSVData& data = csvData.Value();

    // Iterate
    for (Index row = 0; row < data.RowsCount(); ++row)
	{
        jpt::String rowStr;
		for (Index col = 0; col < data[row].Count(); ++col)
		{
			rowStr += data[row][col] + ",";
		}

        //JPT_LOG(rowStr);
	}

    // Single access
    JPT_ENSURE(data[0][0] == "Name");
    JPT_ENSURE(data[0][1] == "Address");
    JPT_ENSURE(data[0][2] == "Phone");
    JPT_ENSURE(data[0][3] == "URL");

    JPT_ENSURE(data[1][0] == "De Sabla, APT");
    JPT_ENSURE(data[1][1] == "10 De Sabla Rd, San Mateo, CA 94402");
    JPT_ENSURE(data[1][2] == "(650) 684-1937");
    JPT_ENSURE(data[1][3] == "https://desabla.com/");

    return true;
}

export bool RunUnitTests_CSV()
{
    JPT_ENSURE(Read());

    return true;
}
