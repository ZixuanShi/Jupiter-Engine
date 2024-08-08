// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input.Backend_Base;

import jpt.Input.KeyCode;
import jpt.TypeDefs;
import jpt.Constants;
import jpt.Time.TypeDefs;

export namespace jpt::Input
{
	class Backend_Base
	{
	public:
		virtual bool IsPressed(KeyCode) const { return false; }
		virtual bool IsReleased(KeyCode) const { return false; }
		virtual bool IsHeld(KeyCode, TimePrecision) const { return false; }

		virtual uint32 FromKeyCode(KeyCode) const { return kInvalidValue<uint32>; }
	};
}