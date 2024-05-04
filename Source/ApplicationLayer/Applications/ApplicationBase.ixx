// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Application.Base;

import jpt.StopWatch;

namespace jpt
{
	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	export class Application_Base
	{
	public:
		virtual ~Application_Base() = default;

		virtual bool Init();
		virtual void Update();
		virtual void Terminate();

		void Run();

		/** Designed to be implemented in client project's application
			@return An Application's reference to the client project. */
		static Application_Base& GetInstance();

	private:
		void PollInput();
		void Render();
	};

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
		
		while (true)
		{
			PollInput();
			Update();
			Render();
		
			if (timer.GetDuration() > 0)
			{
				break;
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