// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

module jpt.BenchmarksReporter;

import jpt.CSV;
import jpt.Clock;
import jpt.DateTime;
import jpt.FilePath;
import jpt.SystemPaths;

namespace jpt
{
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
        JPT_INFO(m_results);
    }
}