// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Application_Base;

class Window_Base;

namespace jpt
{
	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	export class Application_Base
	{
	protected:
		Window_Base* m_pWindow = nullptr;
		bool m_shouldTerminate = false;

	public:
		virtual ~Application_Base() = default;

		virtual bool PreInit();
		virtual bool Init();
		virtual void Update();
		virtual void Terminate();

		void Run();

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
		while (!m_shouldTerminate)
		{
			PollInput();
			Update();
			Render();
		}
	}

	void Application_Base::PollInput()
	{
	}

	void Application_Base::Render()
	{
	}
}