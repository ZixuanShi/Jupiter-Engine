// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Surface;

export namespace jpt
{
	/** Encapsulates a surface that rendering API can draw to */
	class Surface
	{
	public:
		virtual ~Surface() = default;

		virtual bool Init() = 0;
	};
}