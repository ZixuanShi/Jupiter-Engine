// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input.Backend;

import jpt.Input.KeyCode;
import jpt.TypeDefs;
import jpt.Constants;
import jpt.Time.TypeDefs;

export namespace jpt::Input
{
	class Backend
	{
	public:
		virtual ~Backend() = default;

		virtual bool PreInit() { return true; }
		virtual void Update(TimePrecision) {}
		virtual bool Init() { return true; }

		virtual uint32 FromKeyCode(KeyCode) const { return kInvalidValue<uint32>; }
		virtual KeyCode ToKeyCode(uint32) const { return KeyCode::Invalid; }
	};
}