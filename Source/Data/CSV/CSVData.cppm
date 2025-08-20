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

    constexpr String ToString(const CSVData& csvData);
}