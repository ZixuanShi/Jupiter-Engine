// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.TestFramework;

import std;

namespace jpt
{
    TestCase::TestCase(std::string_view name, Function pFunction)
        : m_name(name)
        , m_pFunction(pFunction)
    {
        GetAll().push_back(this);
    }

    Status TestCase::RunAll()
    {
        std::vector<TestCase*>& cases = GetAll();
        std::ranges::sort(cases, {}, [](const TestCase* pCase) { return pCase->m_name; });

        uint32 passedCases = 0;
        uint32 checks = 0;
        uint32 failedChecks = 0;

        // std::println, not Debug::Log: a report is not a debug message, and Logger would stamp
        // every line with this file's location instead of the failing test's.
        std::println("Running {} tests.", cases.size());

        for (TestCase* pCase : cases)
        {
            pCase->m_pFunction(*pCase);

            checks += pCase->m_checks;
            failedChecks += pCase->m_failures;

            const bool passed = pCase->m_failures == 0;
            passedCases += passed;

            std::println("[ {} ] {} ({} checks)", passed ? "PASS" : "FAIL", pCase->m_name, pCase->m_checks);
        }

        std::println("Tests  {}/{} passed", passedCases, cases.size());
        std::println("Checks {}/{} passed", checks - failedChecks, checks);

        return failedChecks == 0 ? Status::Succeeded : Status::Failed;
    }

    std::vector<TestCase*>& TestCase::GetAll()
    {
        static std::vector<TestCase*> s_cases;
        return s_cases;
    }
}
