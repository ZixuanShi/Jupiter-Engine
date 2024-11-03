// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Window;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init(const char* /*title*/, int32 /*width*/ , int32 /*height*/) { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}

		virtual bool ShouldClose() const { return false; }
	};
}