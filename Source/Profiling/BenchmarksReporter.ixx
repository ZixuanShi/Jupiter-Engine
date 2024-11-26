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

export namespace jpt
{
	/** @example
		void ProfileSomething()
		{
			jpt::BenchmarksReporter reporter;
			jpt::StopWatch stopWatch;

			{
				stopWatch.Start();

				// Do some work here
				jpt::SleepMs(1000);

				reporter.Add({ "Test", "Test", stopWatch.GetDurationMs() });
			}
			// Other tests...
			{				
			}

			reporter.Finalize();	// Outputs to CSV file
			reporter.LogResults();	// Outputs to log
		}
	*/
	class BenchmarksReporter
	{
	private:
		CSVData m_results;

	public:
		BenchmarksReporter();

		void Add(const BenchmarkUnit& unit);

		void Finalize();
		void LogResults();
	};

	BenchmarksReporter::BenchmarksReporter()
	{
		// Add Platform, CPU, GPU, Memory

		CSVData::Row header{ "Topic", "Context", "Result (MS)" };
		m_results.AddRow(header);
	}

	void BenchmarksReporter::Add(const BenchmarkUnit& unit)
	{
		m_results.AddRow(unit.ToString());
	}

	void BenchmarksReporter::Finalize()
	{
		const DateTime now = Clock::GetCurrentDateTime();
		const String fileName = "/Benchmarks_" + now.ToFileString() + ".csv";
		const File::Path outputPath = System::Paths::GetInstance().GetSavedDir() + fileName.ConstBuffer();
		WriteCSV(outputPath, m_results);
	}

	void BenchmarksReporter::LogResults()
	{
		JPT_LOG(m_results);
	}
}

