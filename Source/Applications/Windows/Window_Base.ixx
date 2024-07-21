// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Window_Base;

namespace jpt
{
	export class Window_Base
	{
	public:
		virtual ~Window_Base() = default;

		virtual void Create() = 0;
		virtual void Destroy() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
	};
}