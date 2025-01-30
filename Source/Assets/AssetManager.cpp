// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

module jpt.Asset.Manager;

import jpt.Asset;

namespace jpt
{
	bool AssetManager::PreInit()
	{
		return true;
	}

	bool AssetManager::Init()
	{
		return true;
	}

	void AssetManager::Update([[maybe_unused]] TimePrecision deltaSeconds)
	{
	}

	void AssetManager::Shutdown()
	{
		for (auto& [path, pAsset] : m_assets)
		{
			pAsset->Unload();
			delete pAsset;
			pAsset = nullptr;
		}

		m_assets.Clear();
	}

	void AssetManager::Unload(const File::Path& path)
	{
		auto itr = m_assets.Find(path);
		JPT_ASSERT(itr != m_assets.end());

		itr->second->Unload();
		delete itr->second;
		m_assets.Erase(path);
	}
}