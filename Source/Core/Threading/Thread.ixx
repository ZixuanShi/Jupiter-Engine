// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <thread>
#include <atomic>

export module jpt.Thread;

import jpt.String;
import jpt.TypeDefs;
import jpt.Constants;
import jpt.Utilities;

export namespace jpt
{
	/** Base class for all threads
		@example:
		class MyThread : public jpt::Thread_Base */
	class Thread_Base
	{
	protected:
		String m_name = "Unnamed";
		std::atomic_bool m_isRunning = false;

	private:
		std::thread m_thread;

	public:
		Thread_Base() noexcept = default;
		Thread_Base(const char* name) noexcept;

		virtual ~Thread_Base() noexcept;

		Thread_Base(Thread_Base&& other) noexcept;
		Thread_Base& operator=(Thread_Base&& other) noexcept;

		Thread_Base(const Thread_Base&) noexcept = default;
		Thread_Base& operator=(const Thread_Base&)  noexcept = default;

		void Start();
		void Stop();

		const String& GetName() const noexcept;
		uint32 GetId() const noexcept;

	protected:
		virtual void Init() {}
		virtual void Update() {}
		virtual void Terminate() {}
	};

	Thread_Base::Thread_Base(const char* name) noexcept
		: m_name(name) 
	{
	}

	Thread_Base::~Thread_Base() noexcept
	{
		Stop();
	}

	Thread_Base::Thread_Base(Thread_Base&& other) noexcept
		: m_isRunning(other.m_isRunning.load())
		, m_name(Move(other.m_name))
	{
		if (m_isRunning)
		{
			m_thread = Move(other.m_thread);
			other.m_isRunning.store(false);
		}
	}

	Thread_Base& Thread_Base::operator=(Thread_Base&& other) noexcept
	{
		if (this != &other)
		{
			Stop();

			m_name = Move(other.m_name);

			const bool isRunning = other.m_isRunning.load();
			other.m_isRunning.store(false);

			if (isRunning)
			{
				m_thread = Move(other.m_thread);
				m_isRunning.store(true);
			}
		}

		return *this;
	}

	void Thread_Base::Start()
	{
		if (!m_isRunning)
		{
			m_isRunning = true;
			m_thread = std::thread([this]()
				{
					Init();

					while (m_isRunning)
					{
						Update();
					}

					Terminate();
				});
		}
	}

	void Thread_Base::Stop()
	{
		if (m_isRunning)
		{
			m_isRunning = false;
			
			if (m_thread.joinable())
			{
				m_thread.join();
			}
		}
	}

	const String& Thread_Base::GetName() const noexcept
	{
		return m_name;
	}

	uint32 Thread_Base::GetId() const noexcept
	{
		return m_thread.get_id()._Get_underlying_id();
	}
}