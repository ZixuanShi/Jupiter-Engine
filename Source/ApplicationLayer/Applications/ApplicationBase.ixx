// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.ApplicationBase;

import jpt.StopWatch;

namespace jpt
{
	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	export class ApplicationBase
	{
	public:
		virtual ~ApplicationBase() = default;

		virtual bool Init();
		virtual void Update();
		virtual void Clean();

		void Run();

		/** Designed to be implemented in client project's application
			@return An Application's reference to the client project. */
		static ApplicationBase& GetInstance();

	private:
		void PollInput();
		void Render();
	};

	bool ApplicationBase::Init()
	{
		return true;
	}

	void ApplicationBase::Update()
	{
	}

	void ApplicationBase::Clean()
	{

	}

	void ApplicationBase::Run()
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

	void ApplicationBase::PollInput()
	{
	}

	void ApplicationBase::Render()
	{
	}
}