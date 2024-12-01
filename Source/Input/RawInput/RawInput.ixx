// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.RawInput;

import jpt.Input.KeyCode;

import jpt.Constants;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

using namespace jpt::Input;

export namespace jpt
{
	/** Responsible for low-level input query.
		- Input State per input device, per window
		- Converts Jupiter Engine's Key Code to/from framework Key Code */
	class RawInput
	{
	public:
		virtual ~RawInput() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update([[maybe_unused]] TimePrecision deltaSeconds) {}
		virtual void Shutdown() {}

	public:
		virtual uint32 FromKeyCode([[maybe_unused]] KeyCode key) const { return kInvalidValue<uint32>; }
		virtual KeyCode ToKeyCode([[maybe_unused]] uint32 key) const { return KeyCode::Invalid; }
	};
}