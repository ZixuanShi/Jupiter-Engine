// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <condition_variable>

export module jpt.ConditionVariable;

import jpt.Mutex;

export namespace jpt
{
    /** A synchronization primitive that can be used to block a thread, or multiple threads at the same time, until another thread both modifies a shared variable (the condition), and notifies the condition_variable.
        @example:
            jpt::Mutex mutex;
            jpt::ConditionVariable conditionVariable;
            void ThreadFunction()
            {
                std::unique_lock<std::mutex> lock(mutex);
                conditionVariable.Wait(lock);
            }
            void NotifyFunction()
            {
                conditionVariable.NotifyOne();
            } */
    class ConditionVariable
    {
    private:
        std::condition_variable m_conditionVariable;

    public:
        ConditionVariable() = default;
        ~ConditionVariable() = default;

        void NotifyOne();
        void NotifyAll();

        void Wait(std::unique_lock<std::mutex>& lock);

        template<typename Predicate>
        void Wait(std::unique_lock<std::mutex>& lock, Predicate predicate);
    };

    template<typename Predicate>
    void ConditionVariable::Wait(std::unique_lock<std::mutex>& lock, Predicate predicate)
    {
        m_conditionVariable.wait(lock, predicate);
    }
}