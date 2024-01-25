// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TimePoint;

import jpt.TimePointData;
import jpt.TypeDefs;
import jpt.ToString;

export namespace jpt
{
	/** Represents a point in time */
	class TimePoint
	{
	private:
		TimePointData m_data;

	public:
		TimePoint() = default;
		TimePoint(TimePointData data) : m_data(data) {}

		/** @return		A string showing time point info with the default format of "MM/DD/YYYY. HH:MM:SS" */
		String ToString() const { return m_data.ToString(); }
	};
}