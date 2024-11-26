// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.BenchmarkUnit;

import jpt.String;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;
import jpt.Function;

export namespace jpt
{
	struct BenchmarkUnit
	{
		String topic;
		String context;
		TimePrecision resultMS;

		String ToString() const
		{
			return String::Format<256>("%s,%s,%f", topic.ConstBuffer(), context.ConstBuffer(), resultMS);			
		}
	};
}