// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Window;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Window
	{
	protected:
		const char* m_pTitle = "Unnamed";

	public:
		virtual ~Window() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}
	};
}