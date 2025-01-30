// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Asset;

import jpt.File.Path;

export namespace jpt
{
	/** Base class for assets */
	class Asset
	{
	public:
		virtual ~Asset() = default;

		virtual bool Load(const File::Path&) { return false; } 
		virtual void Unload() {}
	};
}