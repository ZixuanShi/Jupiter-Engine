// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

export module jpt.BenchmarksReporter;

export import jpt.BenchmarkUnit;
export import jpt.StopWatch;

import jpt.CSV;
import jpt.CSV.Data;
import jpt.Clock;
import jpt.DateTime;
import jpt.File.Path;
import jpt.System.Paths;
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

    BenchmarksReporter::BenchmarksReporter()
    {
        // Add Platform, CPU, GPU, Memory

        CSVData::Row header{ "Topic", "Context", "Result (MS)" };
        m_results.AddRow(header);
    }

    void BenchmarksReporter::Profile(const char* topic, const char* context, size_t count, const Function<void()>& func)
    {
        StopWatch::Point now;
        TimePrecision result = 0.0;
        {
            now = StopWatch::Now();
            for (size_t i = 0; i < count; ++i)
            {
                func();
            }
            result = StopWatch::GetMsFrom(now);
        }
        BenchmarkUnit unit{ topic, context, result };
        m_results.AddRow(ToString(unit));
    }

    void BenchmarksReporter::Finalize()
    {
        const DateTime now = Clock::GetCurrentDateTime();
        const String fileName = "/Benchmarks_" + ToFileString(now) + ".csv";
        const File::Path outputPath = System::Paths::GetInstance().GetSavedDir() + fileName.ConstBuffer();
        WriteCSV(outputPath, m_results);
    }

    void BenchmarksReporter::LogResults()
    {
        JPT_LOG(m_results);
    }
}

