// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <condition_variable>
#include <mutex>

export module jpt.ConditionVariable;

import jpt.Mutex;

export namespace jpt
{
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

	template<typename Predicate>
	void ConditionVariable::Wait(std::unique_lock<std::mutex>& lock, Predicate predicate)
	{
		m_conditionVariable.wait(lock, predicate);
	}
}