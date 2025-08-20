// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.CSVData;

namespace jpt
{
    constexpr void CSVData::AddRow(const Row& row)
    {
        m_rows.EmplaceBack(row);
    }

    constexpr void CSVData::AddRow(const String& row)
    {
        m_rows.EmplaceBack(row.Split(','));
    }

    constexpr void CSVData::Reserve(Index rowsCount)
    {
        m_rows.Reserve(rowsCount);
    }

    constexpr CSVData::Row& CSVData::operator[](Index index)
    {
        return m_rows[index];
    }

    constexpr const CSVData::Row& CSVData::operator[](Index index) const
    {
        return m_rows[index];
    }

    constexpr Index CSVData::RowsCount() const
    {
        return m_rows.Count();
    }

    constexpr String ToString(const CSVData& csvData)
    {
        String content;

        for (Index row = 0; row < csvData.RowsCount(); ++row)
        {
            for (Index col = 0; col < csvData[row].Count(); ++col)
            {
                String cell = csvData[row][col];
                if (cell.Find(',') != npos)
                {
                    cell = "\"" + cell + "\"";
                }

                content += cell;
                if (col < csvData[row].Count() - 1)
                {
                    content += ",";
                }
            }
            content += "\n";
        }

        return content;
    }
}