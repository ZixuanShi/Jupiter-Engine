// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

export module jpt.Asset.Manager;

import jpt.TypeDefs;
import jpt.String;
import jpt.HashMap;
import jpt.File.Path;

export namespace jpt
{
	class Asset;

	class AssetManager
	{
	private:
		HashMap<File::Path, Asset*> m_assets;

	public:

	};
}