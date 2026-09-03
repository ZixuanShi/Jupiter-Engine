// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TestFramework;

import jpt.DebugContext;
import jpt.Logger;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** One case's checks. A case is a function taking the TestCase it reports to, and nothing else:

            export void RunUnitTests_Vector2(jpt::TestCase& test) { test.Expect(...); }

        Its category's aggregator is what names it and runs it, so the name lives at the call site
        rather than in a wrapper beside every case:

            jpt::TestCase::Run("Math.Vector2", &RunUnitTests_Vector2);

        Nothing registers itself, so a run executes in the order the aggregators list it -- see
        Source/ApplicationUnitTests.cpp for the top of that list. */
    class TestCase
    {
    public:
        using Function = void(*)(TestCase&);

    private:
        std::string_view m_name;
        uint32 m_checks = 0;
        uint32 m_failures = 0;

    public:
        /** Runs pFunction against a fresh case, prints its [ PASS ] / [ FAIL ] line and folds its
            checks into the run's totals. Every RunUnitTests_ entry is a list of these. */
        static void Run(std::string_view name, Function pFunction);

        /** Debug::Assert without the trap: logs the failure at the call site and records it, then
            lets the test carry on. @return the condition, so a test can still bail on its own. */
        template<typename... Args>
        bool Expect(bool condition, Debug::Context<std::type_identity_t<Args>...> context, Args&&... args);

        /** The run's two totals lines, and its verdict. Called once, by ApplicationUnitTests::Init.
            @return Succeeded, or Failed if any check failed, which drives the exit code. */
        [[nodiscard]] static Status Summarize();

    private:
        explicit TestCase(std::string_view name);
    };

    template<typename... Args>
    bool TestCase::Expect(bool condition, Debug::Context<std::type_identity_t<Args>...> context, Args&&... args)
    {
        ++m_checks;

        if (!condition) [[unlikely]]
        {
            ++m_failures;
            Debug::Error(context, std::forward<Args>(args)...);
        }

        return condition;
    }
}
