// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.ApplicationBase;

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

		/** Designed to be implemented in client project's application
			@return An Application's reference to the client project. */
		static ApplicationBase& GetInstance();
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
}
