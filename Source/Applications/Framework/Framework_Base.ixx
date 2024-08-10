// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Framework_Base;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Application_Base;

	class Framework_Base
	{
	protected:
		Application_Base* m_pApp = nullptr;

	public:
		virtual ~Framework_Base() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init(Application_Base* pApp);
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}
	};

	bool Framework_Base::Init(Application_Base* pApp)
	{
		JPT_ASSERT(pApp);

		m_pApp = pApp;
		return true;
	}
}