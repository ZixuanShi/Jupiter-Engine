// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.BenchmarkUnit;

import jpt.String;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
	struct BenchmarkUnit
	{
		String topic;
		String context;
		TimePrecision resultMS;
		TimePrecision stdComparison;	// Optional

		String ToString() const
		{
			return String::Format<256>("%s,%s,%f,%f", topic.ConstBuffer(), context.ConstBuffer(), resultMS, stdComparison);			
		}
	};
}