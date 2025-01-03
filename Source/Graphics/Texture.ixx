// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

export module jpt.Texture;

import jpt.File.Path;

export namespace jpt
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual bool Load(const File::Path& fullPath) = 0;
		virtual void Shutdown() = 0;
	};
}