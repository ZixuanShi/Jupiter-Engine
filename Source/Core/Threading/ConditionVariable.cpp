// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <condition_variable>
#include <mutex>

module jpt.ConditionVariable;

namespace jpt
{
    void ConditionVariable::NotifyOne()
    {
        m_conditionVariable.notify_one();
    }

    void ConditionVariable::NotifyAll()
    {
        m_conditionVariable.notify_all();
    }

    void ConditionVariable::Wait(std::unique_lock<std::mutex>& lock)
    {
        m_conditionVariable.wait(lock);
    }
}