// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

export module jpt.AssetManager;

import jpt.TypeDefs;
import jpt.String;
import jpt.HashMap;
import jpt.FilePath;

export namespace jpt
{
    class Asset;

    class AssetManager
    {
        JPT_DECLARE_SINGLETON(AssetManager);

    private:
        HashMap<File::Path, Asset*> m_assets;

    public:
        bool PreInit();
        bool Init();
        void Terminate();

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

        TAsset* pAsset = Allocator<TAsset>::New();
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