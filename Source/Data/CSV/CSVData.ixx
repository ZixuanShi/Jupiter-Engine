// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

export module jpt.CSV.Data;

import jpt.DynamicArray;
import jpt.String;

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
		constexpr void Reserve(Index rowsCount);

		constexpr       Row& operator[](Index index);
		constexpr const Row& operator[](Index index) const;

		constexpr Index RowsCount() const { return m_rows.Count(); }
	};

	constexpr void CSVData::AddRow(const Row& row)
	{
		m_rows.EmplaceBack(row);
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
}