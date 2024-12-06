// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module jpt.Framework_Win32;

import jpt.Framework;

namespace jpt
{
	export class Framework_Win32 final : public Framework
	{
		using Super = Framework;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;
	};

	bool Framework_Win32::Init()
	{
		JPT_ENSURE(Super::Init());
		return true;
	}

	void Framework_Win32::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

	}

	void Framework_Win32::Shutdown()
	{


		Super::Shutdown();
	}
}