// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.CSVData;

import jpt.DynamicArray;
import jpt.String;
import jpt.TypeDefs;

export namespace jpt
{
    class CSVData
    {
    public:
        using Row = DynamicArray<String>;
        using Rows = DynamicArray<Row>;

    private:
        Rows m_rows;

    public:
        constexpr CSVData() = default;

        constexpr void AddRow(const Row& row);

        /** @param row        Already comma separated formatted string */
        constexpr void AddRow(const String& row);

        constexpr void Reserve(Index rowsCount);

        constexpr       Row& operator[](Index index);
        constexpr const Row& operator[](Index index) const;

        constexpr Index RowsCount() const;
    };

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