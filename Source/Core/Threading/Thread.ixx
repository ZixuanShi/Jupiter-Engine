// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <thread>
#include <atomic>

export module jpt.Thread;

import jpt.String;
import jpt.TypeDefs;
import jpt.Constants;

export namespace jpt
{
	/** Base class for all threads
		@example:
		class MyThread : public jpt::Thread_Base */
	class Thread_Base
	{
	private:
		std::thread m_thread;
		std::atomic_bool m_isRunning = false;

	public:
		virtual ~Thread_Base();

		void Start();
		void Stop();

	protected:
		virtual void Init() {}
		virtual void Update() = 0;
		virtual void Terminate() {}

	private:
		void Run();
	};

	Thread_Base::~Thread_Base()
	{
		Stop();
	}

	void Thread_Base::Start()
	{
		if (!m_isRunning)
		{
			m_isRunning = true;
			m_thread = std::thread(&Thread_Base::Run, this);
		}
	}

	void Thread_Base::Stop()
	{
		if (m_isRunning)
		{
			m_isRunning = false;
			m_thread.join();
		}
	}

	void Thread_Base::Run()
	{
		Init();

		while (m_isRunning)
		{
			Update();
		}

		Terminate();
	}
}