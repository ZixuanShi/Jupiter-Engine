// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LockGuard;

import jpt.Mutex;

export namespace jpt
{
	/** A simple RAII wrapper for Mutex that automatically locks on construction and unlocks on destruction. */
	class LockGuard
	{
	private:
		Mutex& m_mutex;

	public:
		LockGuard(Mutex& mutex);
		~LockGuard();
	};

	LockGuard::LockGuard(Mutex& mutex)
		: m_mutex(mutex)
	{
		m_mutex.Lock();
	}

	LockGuard::~LockGuard()
	{
		m_mutex.Unlock();
	}
}