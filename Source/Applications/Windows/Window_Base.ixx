// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application_Base.h"
#include "Debugging/Assert.h"

export module jpt.Window_Base;

namespace jpt
{
	export class Window_Base
	{
	protected:
		Application_Base* m_pApp = nullptr;

	public:
		virtual ~Window_Base() = default;

		virtual bool Init(Application_Base* pApp);
		virtual void Update() {}
		virtual void Terminate() {}
	};

	bool Window_Base::Init(Application_Base* pApp)
	{
		JPT_ASSERT(pApp);

		m_pApp = pApp;
		return true;
	}
}