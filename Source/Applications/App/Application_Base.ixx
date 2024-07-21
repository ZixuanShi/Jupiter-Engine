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
		static Application_Base* GetInstance();

		virtual bool PreInit()   { return true; }
		virtual bool Init()      { return true; }
		virtual void Update()    { }
		virtual void Terminate() { }

		void Run();

	protected:
		void PollInput() { }
		void Render()    { }
	};

	void Application_Base::Run()
	{
		while (!m_shouldTerminate)
		{
			PollInput();
			Update();
			Render();
		}
	}
}