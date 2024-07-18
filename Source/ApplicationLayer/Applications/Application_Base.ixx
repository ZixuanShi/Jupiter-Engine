// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Application_Base;

import jpt.StopWatch;

namespace jpt
{
	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	export class Application_Base
	{
	protected:
		bool m_shouldTerminate = false;

	public:
		virtual ~Application_Base() = default;

		virtual bool PreInit();
		virtual bool Init();

		virtual void Update();
		virtual void Terminate();

		void Run();

		/** Designed to be implemented in client project's application
			@return An Application's reference to the client project. */
		static Application_Base* GetInstance();

	protected:
		void PollInput();
		void Render();
	};

	bool Application_Base::PreInit()
	{
		return true;
	}

	bool Application_Base::Init()
	{
		return true;
	}

	void Application_Base::Update()
	{
	}

	void Application_Base::Terminate()
	{

	}

	void Application_Base::Run()
	{
		StopWatch timer;
		timer.Start();
		
		while (!m_shouldTerminate)
		{
			PollInput();
			Update();
			Render();
		
			if (timer.GetDuration() > 0)
			{
				m_shouldTerminate = true;
			}
		}
	}

	void Application_Base::PollInput()
	{
	}

	void Application_Base::Render()
	{
	}
}