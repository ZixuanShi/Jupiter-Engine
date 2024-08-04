// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

export module jpt.ThreadSafeQueue;

import jpt.Mutex;
import jpt.Atomic;
import jpt.LockGuard;
import jpt.ConditionVariable;
import jpt.Queue;
import jpt.Optional;
import jpt.Utilities;

export namespace jpt
{
	template<typename T>
	class ThreadSafeQueue
	{
	private:
		Mutex m_mutex;
		Queue<T> m_queue;
		ConditionVariable m_conditionVariable;
		Atomic<bool> m_shouldTerminate{ false };

	public:
		ThreadSafeQueue() = default;

		void Push(const T& value);
		void Push(T&& value);

		Optional<T> TryPop();
		Optional<T> WaitPop();

		void Terminate();

		bool IsEmpty() const;
	};

	template<typename T>
	void ThreadSafeQueue<T>::Push(const T& value)
	{
		LockGuard lock(m_mutex);
		m_queue.Enqueue(value);
		m_conditionVariable.NotifyOne();
	}

	template<typename T>
	void ThreadSafeQueue<T>::Push(T&& value)
	{
		LockGuard lock(m_mutex);
		m_queue.Enqueue(Move(value));
		m_conditionVariable.NotifyOne();
	}

	template<typename T>
	Optional<T> ThreadSafeQueue<T>::TryPop()
	{
		LockGuard lock(m_mutex);

		if (m_queue.IsEmpty())
		{
			return {};
		}

		T value = Move(m_queue.Front());
		m_queue.Dequeue();
		return value;
	}

	template<typename T>
	Optional<T> ThreadSafeQueue<T>::WaitPop()
	{
		auto lock = m_mutex.CreateUniqueLock();

		m_conditionVariable.Wait(lock, [this] { return !m_queue.IsEmpty() || m_shouldTerminate; });

		if (m_queue.IsEmpty() && m_shouldTerminate)
		{
			return {};
		}

		T value = Move(m_queue.Front());
		m_queue.Dequeue();
		return value;
	}

	template<typename T>
	void ThreadSafeQueue<T>::Terminate()
	{
		LockGuard lock(m_mutex);
		m_shouldTerminate = true;
		m_conditionVariable.NotifyAll();
	}

	template<typename T>
	bool ThreadSafeQueue<T>::IsEmpty() const
	{
		LockGuard lock(m_mutex);
		return m_queue.IsEmpty();
	}
}