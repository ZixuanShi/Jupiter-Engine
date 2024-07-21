// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Framework_Base;

namespace jpt
{
	export class Framework_Base
	{
	public:
		virtual ~Framework_Base() = default;

		virtual bool Init() { return true; }
		virtual void Update() {}
		virtual void Terminate() {}
	};
}