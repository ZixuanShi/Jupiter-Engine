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
        CSVData() = default;

        void AddRow(const Row& row);

        /** @param row        Already comma separated formatted string */
        void AddRow(const String& row);

        void Reserve(Index rowsCount);

              Row& operator[](Index index);
        const Row& operator[](Index index) const;

        Index RowsCount() const;
    };

    String ToString(const CSVData& csvData);
}