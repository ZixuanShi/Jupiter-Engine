// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

module jpt.AssetManager;

import jpt.Application;
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

    void AssetManager::Terminate()
    {
        for (auto& [path, pAsset] : m_assets)
        {
            pAsset->Unload();
            Allocator<Asset>::Delete(pAsset);
            pAsset = nullptr;
        }

        m_assets.Clear();
    }

    void AssetManager::Unload(const File::Path& path)
    {
        auto itr = m_assets.Find(path);
        JPT_ASSERT(itr != m_assets.end());

        itr->second->Unload();
        Allocator<Asset>::Delete(itr->second);
        m_assets.Erase(path);
    }
}