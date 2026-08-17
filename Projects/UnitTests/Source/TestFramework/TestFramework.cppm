// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TestFramework;

import jpt.DebugContext;
import jpt.Logger;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** A test, and the set of every test. Declaring one at namespace scope is the registration:

            static TestCase s_vector2("Math.Vector2", &Vector2Test);

        Nothing imports a test file, so what runs it is its object being named on the executable's
        link line -- where this constructor runs during static initialisation. */
    class TestCase
    {
    public:
        using Function = void(*)(TestCase&);

    private:
        std::string_view m_name;
        Function m_pFunction = nullptr;
        uint32 m_checks = 0;
        uint32 m_failures = 0;

    public:
        TestCase(std::string_view name, Function pFunction);

        /** Debug::Assert without the trap: logs the failure at the call site and records it, then
            lets the test carry on. @return the condition, so a test can still bail on its own. */
        template<typename... Args>
        bool Expect(bool condition, Debug::Context<std::type_identity_t<Args>...> context, Args&&... args);

        /** Runs every test in name order -- static-init order across translation units is
            unspecified, so sorting is what makes a run reproducible.
            @return Succeeded, or Failed if any check failed, which drives the exit code. */
        [[nodiscard]] static Status RunAll();

    private:
        /** Function-local static, so a TestCase constructed during static initialisation cannot
            observe it unconstructed. Holds pointers to objects that outlive the run. */
        static std::vector<TestCase*>& GetAll();
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
