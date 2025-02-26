// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

export module jpt.CSV;

import jpt.TypeDefs;
import jpt.Optional;

import jpt.File.Path;
import jpt.File.IO;

import jpt.CSV.Data;

using namespace jpt::File;

export namespace jpt
{
	Optional<CSVData> ReadCSV(const Path& path)
	{
		Optional<String> csvContent = ReadTextFile(path);
		JPT_ASSERT(csvContent, "Failed to read CSV file as text at %ls", path.ConstBuffer());

		const String& content = csvContent.Value();
		if (content.IsEmpty())
		{
			return {};
		}

		CSVData csvData;

		// Get each line
		DynamicArray<String> rows = content.Split('\n');
		csvData.Reserve(rows.Count());

		for (String& row : rows)
		{
			// For each line, parse each cell and add to current row. 
			CSVData::Row currentRow;
			while (true)
			{
				// Break if the row is empty
				if (row.IsEmpty())
				{
					break;
				}

				const Index quoteStartIndex = row.Find('\"');

				// If there's no quote, split this row by comma and break
				if (quoteStartIndex == npos)
				{
					currentRow += row.Split(',');
					break;
				}

				// If there's a quote
				const Index commaIndex = row.Find(',');
				if (quoteStartIndex != npos)
				{
					// Quoted cell is at the start of the row
					if (quoteStartIndex < commaIndex)
					{
						const Index quoteEndIndex = row.Find('\"', quoteStartIndex + 1);
						JPT_ASSERT(quoteEndIndex != npos, "Failed to find closing quote in CSV row");

						const String cell = row.SubStr(quoteStartIndex + 1, quoteEndIndex - quoteStartIndex - 1);
						currentRow.EmplaceBack(cell);

						row = row.SubStr(quoteEndIndex + 2);	// +2 for the comma and the space
					}
					// Quoted cell is not current cell
					else
					{
						currentRow.EmplaceBack(row.SubStr(0, commaIndex));
						row = row.SubStr(commaIndex + 1);
					}
				}
			}

			csvData.AddRow(currentRow);
		}

		return csvData;
	}

	void WriteCSV(const Path& path, const CSVData& data)
	{
		WriteTextFile(path, ToString(data));
	}
}