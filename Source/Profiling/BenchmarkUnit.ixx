// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.BenchmarkUnit;

import jpt.String;
import jpt.TypeDefs;
import jpt.TimeTypeDefs;
import jpt.Function;

export namespace jpt
{
    struct BenchmarkUnit
    {
        String topic;
        String context;
        TimePrecision resultMS;
    };

    String ToString(const BenchmarkUnit& unit)
    {
        return String::Format<256>("%s,%s,%f", unit.topic.ConstBuffer(), unit.context.ConstBuffer(), unit.resultMS);
    }
}