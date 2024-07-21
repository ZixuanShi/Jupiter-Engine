// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Window_Base;

namespace jpt
{
	export class Window_Base
	{
	public:
		virtual ~Window_Base() = default;

		virtual bool Init() { return true; }
		virtual void Update() {}
		virtual void Terminate() {}
	};
}