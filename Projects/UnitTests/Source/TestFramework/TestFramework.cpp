// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.TestFramework;

import std;

// The run's tally, not a registry: nothing here decides what runs, only records what has run.
uint32 g_cases = 0;
uint32 g_passedCases = 0;
uint32 g_checks = 0;
uint32 g_failedChecks = 0;

namespace jpt
{
    TestCase::TestCase(std::string_view name)
        : m_name(name)
    {
    }

    void TestCase::Run(std::string_view name, Function pFunction)
    {
        TestCase test(name);
        pFunction(test);

        const bool passed = test.m_failures == 0;

        ++g_cases;
        g_passedCases += passed;
        g_checks += test.m_checks;
        g_failedChecks += test.m_failures;

        // std::println, not Debug::Log: a report is not a debug message, and Logger would stamp
        // every line with this file's location instead of the failing test's.
        std::println("[ {} ] {} ({} checks)", passed ? "PASS" : "FAIL", test.m_name, test.m_checks);
    }

    Status TestCase::Summarize()
    {
        std::println("Tests  {}/{} passed", g_passedCases, g_cases);
        std::println("Checks {}/{} passed", g_checks - g_failedChecks, g_checks);

        return g_failedChecks == 0 ? Status::Succeeded : Status::Failed;
    }
}
