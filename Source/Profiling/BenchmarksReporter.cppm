// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.BenchmarksReporter;

export import jpt.BenchmarkUnit;
export import jpt.StopWatch;

import jpt.CSVData;
import jpt.Function;

export namespace jpt
{
    class BenchmarksReporter
    {
    private:
        CSVData m_results;

    public:
        BenchmarksReporter();

        void Profile(const char* topic, const char* context, size_t count, const Function<void()>& func);

        void Finalize();
        void LogResults();
    };
}

