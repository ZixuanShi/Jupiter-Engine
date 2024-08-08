// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Input.Backend_Base;

import jpt.Input.KeyCode;
import jpt.TypeDefs;
import jpt.Constants;

export namespace jpt::Input
{
	class Backend_Base
	{
	public:
		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update() { }
		virtual void Terminate() { }

		virtual bool IsKeyPressed(KeyCode) const { return false; }
		virtual uint32 FromKeyCode(KeyCode) const { return kInvalidValue<uint32>; }
	};
}