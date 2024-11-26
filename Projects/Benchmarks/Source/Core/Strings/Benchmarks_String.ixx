// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Profiling/TimingProfiler.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"
#include "Core/Minimal/CoreMacros.h"

#include <string>

export module Benchmarks_String;

import jpt.BenchmarksReporter;
import jpt.DynamicArray;
import jpt.String;
import jpt.ToString;
import jpt.TypeDefs;

void Find(jpt::BenchmarksReporter& reporter)
{
	reporter.Profile("String", "Find 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "Hello Jupiter World";
			JPT_ASSERT(str.Find("Jupiter") == 6);
			JPT_ASSERT(str.Find("NonExist") == jpt::npos);
		});
}

void Replace(jpt::BenchmarksReporter& reporter)
{
	reporter.Profile("String", "Replace 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "Hello World o";
			str.Replace("o", "Jupiter");
			JPT_ASSERT(str == "HellJupiter WJupiterrld Jupiter");
			str.Replace("Jupiter", "o");
			JPT_ASSERT(str == "Hello World o");
		});
}

void SubStr(jpt::BenchmarksReporter& reporter)
{
	reporter.Profile("String", "SubStr 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "Hello Jupiter World";
			JPT_ASSERT(str.SubStr(6, 7) == "Jupiter");
		});
}

void Split(jpt::BenchmarksReporter& reporter)
{
	reporter.Profile("String", "Split 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "Hello Jupiter World";
			const jpt::DynamicArray<jpt::String> split = str.Split(" ");
			JPT_ASSERT(split.Count() == 3);
			JPT_ASSERT(split[0] == "Hello");
			JPT_ASSERT(split[1] == "Jupiter");
			JPT_ASSERT(split[2] == "World");
		});
}

void Insert(jpt::BenchmarksReporter& reporter)
{
	reporter.Profile("String", "Insert 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "Hello World";
			str.Insert(" Jupiter", 5);
			JPT_ASSERT(str == "Hello Jupiter World");
		});
}

void TrimLeft(jpt::BenchmarksReporter& reporter)
{
	reporter.Profile("String", "TrimLeft 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "   Hello Jupiter World";
			str.TrimLeft();
			JPT_ASSERT(str == "Hello Jupiter World");
		});
}

void TrimRight(jpt::BenchmarksReporter& reporter)
{
	reporter.Profile("String", "TrimRight 1'000'000 elements", 1'000'000, []()
		{
			jpt::String str = "Hello Jupiter World   ";
			str.TrimRight();
			JPT_ASSERT(str == "Hello Jupiter World");
		});
}

export void RunBenchmarks_String(jpt::BenchmarksReporter& reporter)
{
	//Find(reporter);
	//Replace(reporter);
	//SubStr(reporter);
	//Split(reporter);
	//Insert(reporter);
	TrimLeft(reporter);
	TrimRight(reporter);
}