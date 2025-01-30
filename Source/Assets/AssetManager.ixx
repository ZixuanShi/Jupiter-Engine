// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

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
		bool PreInit();
		bool Init();
		void Update(TimePrecision deltaSeconds);
		void Shutdown();

	public:
		template<typename TAsset>
		TAsset* Load(const File::Path& path);

		void Unload(const File::Path& path);

		template<typename TAsset>
		TAsset* Get(const File::Path& path);
	};

	template<typename TAsset>
	TAsset* AssetManager::Load(const File::Path& path)
	{
		JPT_ASSERT(!m_assets.Has(path));

		TAsset* pAsset = new TAsset();
		if (!pAsset->Load(path))
		{
			delete pAsset;
			return nullptr;
		}

		m_assets.Emplace(path, pAsset);
		return pAsset;
	}

	template<typename TAsset>
	TAsset* AssetManager::Get(const File::Path& path)
	{
		JPT_ASSERT(m_assets.Has(path));
		return static_cast<TAsset*>(m_assets[path]);
	}
}