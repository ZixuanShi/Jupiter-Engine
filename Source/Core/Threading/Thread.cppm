// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <thread>
#include <atomic>

export module jpt.Thread;

import jpt.Atomic;
import jpt.String;
import jpt.TypeDefs;
import jpt.UniquePtr;

export namespace jpt
{
    /** Base class for all threads
        @example:
            class TestThread final : public jpt::Thread
            {
            protected:
                void Init() override 
                {
                    JPT_LOG("Initializing thread " + m_name + jpt::ToString(GetId()));
                }

                void Update() override 
                {
                    JPT_LOG("Updating thread " + m_name + jpt::ToString(GetId()));
                    jpt::Sleep(1);
                }

                void Terminate() override 
                {
                    JPT_LOG("Terminating thread " + m_name + jpt::ToString(GetId()));
                }
            }; */
    class Thread
    {
    protected:
        String m_name = "Unnamed";
        Atomic<bool> m_isActive{ false };

    private:
        UniquePtr<std::thread> m_thread;

    public:
        Thread() noexcept = default;
        Thread(const char* name) noexcept;
        virtual ~Thread() noexcept;

        Thread(Thread&& other) noexcept;
        Thread& operator=(Thread&& other) noexcept;

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        void Start();
        void Stop();

        const String& GetName() const noexcept;

    protected:
        virtual void Init() {}
        virtual void Update() {}
        virtual void Terminate() {}
    };
}