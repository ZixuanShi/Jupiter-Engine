// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Window_Base;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Application_Base;

	class Window_Base
	{
	protected:
		Application_Base* m_pApp = nullptr;

	public:
		virtual ~Window_Base() = default;

		virtual bool Init(Application_Base* pApp);
		virtual void Update(TimePrecision) {}
		virtual void Terminate() {}
	};

	bool Window_Base::Init(Application_Base* pApp)
	{
		JPT_ASSERT(pApp);

		m_pApp = pApp;
		return true;
	}
}