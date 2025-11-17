// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"
#include "Profiling/TimingProfiler.h"

#include <memory>

export module Benchmarks_Utilities;

import jpt.BenchmarksReporter;
import jpt.TypeDefs;
import jpt.Byte;

constexpr size_t kCount = 1000;
constexpr size_t kSize = 1024 * 1024; // 1 MB
jpt::Byte* g_pSrc = nullptr;
jpt::Byte* g_pDest = nullptr;
jpt::Byte* g_pBuffer = nullptr;

void jptMemcpy(jpt::BenchmarksReporter& reporter)
{
    reporter.Profile("Utilities", "jpt::MemCpy", kCount, [&]()
        {
            jpt::MemCpy(g_pDest, g_pSrc, kSize);
        });
}

void stdMemcpy(jpt::BenchmarksReporter& reporter)
{
    reporter.Profile("Utilities", "std::memcpy", kCount, [&]()
        {
            std::memcpy(g_pDest, g_pSrc, kSize);
        });
}

void jptMemMove(jpt::BenchmarksReporter& reporter)
{
    reporter.Profile("Utilities", "jpt::MemMove", kCount, [&]()
        {
            jpt::MemMove(g_pBuffer + 256, g_pBuffer, kSize); // Overlapping regions
        });
}

void stdMemMove(jpt::BenchmarksReporter& reporter)
{
    reporter.Profile("Utilities", "std::memmove", kCount, [&]()
        {
            std::memmove(g_pBuffer + 256, g_pBuffer, kSize); // Overlapping regions
        });
}

export void RunBenchmarks_Utilities(jpt::BenchmarksReporter& reporter)
{
    g_pSrc = new jpt::Byte[kSize];
    g_pDest = new jpt::Byte[kSize];
    g_pBuffer = new jpt::Byte[kSize + 512]; // Extra space for overlap

    jptMemcpy(reporter);
    stdMemcpy(reporter);

    jptMemMove(reporter);
    stdMemMove(reporter);

    JPT_DELETE(g_pSrc);
    JPT_DELETE(g_pDest);
    JPT_DELETE(g_pBuffer);
}