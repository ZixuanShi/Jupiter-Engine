// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_CSV;

import jpt.CSV;
import jpt.CSV.Data;

import jpt.TypeDefs;
import jpt.Optional;
import jpt.Utilities;

import jpt.File.Path;
import jpt.File.Path.Utils;
import jpt.File.IO;
import jpt.File.Enums;

const jpt::File::Path kCSVPath = jpt::File::Combine(jpt::File::Source::Output, "Assets/Test.csv");

static bool Write()
{
    jpt::CSVData csvData;

    csvData.Reserve(5);
    csvData.AddRow({ "Name", "Address", "Phone", "URL" });
    csvData.AddRow({ "De Sabla, APT", "10 De Sabla Rd, San Mateo, CA 94402", "(650) 684-1937", "https://desabla.com/" });
    csvData.AddRow({ "Westlake", "331 Park Plaza Dr, Daly City, CA 94015", "(650) 755-8133", "https://www.westlakedalycity.com/" });
    csvData.AddRow({ "Skyline Heights", "960 St Francis Blvd, Daly City, CA 94015", "(650) 590-0714", "https://www.skylineheights.com/" });
    csvData.AddRow({ "Soma Square", "1 Saint Francis Pl, San Francisco, CA 94107", "(415) 284-3000", "https://www.equityapartments.com/san-francisco-bay/soma/soma-square-apartments" });
    
    jpt::WriteCSV(kCSVPath, csvData);

    return true;
}

static bool Read()
{
    jpt::Optional<jpt::CSVData> csvData = jpt::ReadCSV(kCSVPath);
    JPT_ENSURE(csvData);

    jpt::CSVData& data = csvData.Value();

    // Iterate
    for (Index row = 0; row < data.RowsCount(); ++row)
    {
        for (Index col = 0; col < data[row].Count(); ++col)
        {
            JPT_IGNORE(data[row][col]);
        }
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

    JPT_ENSURE(data[2][0] == "Westlake");
    JPT_ENSURE(data[2][1] == "331 Park Plaza Dr, Daly City, CA 94015");
    JPT_ENSURE(data[2][2] == "(650) 755-8133");
    JPT_ENSURE(data[2][3] == "https://www.westlakedalycity.com/");

    JPT_ENSURE(data[3][0] == "Skyline Heights");
    JPT_ENSURE(data[3][1] == "960 St Francis Blvd, Daly City, CA 94015");
    JPT_ENSURE(data[3][2] == "(650) 590-0714");
    JPT_ENSURE(data[3][3] == "https://www.skylineheights.com/");

    return true;
}

export bool RunUnitTests_CSV()
{
    JPT_ENSURE(Write());
    JPT_ENSURE(Read());
    jpt::File::Delete(kCSVPath);

    return true;
}
