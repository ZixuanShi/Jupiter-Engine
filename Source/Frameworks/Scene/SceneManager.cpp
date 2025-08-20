// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"
#include "Core/Validation/Assert.h"

module jpt.SceneManager;

import jpt.SceneGraph;

namespace jpt
{
    bool SceneManager::PreInit()
    {
        return true;
    }

    bool SceneManager::Init()
    {
        return true;
    }

    void SceneManager::Update(TimePrecision deltaSeconds)
    {
#if !IS_EDITOR
        //JPT_ASSERT(m_pCurrentScene, "Current scene is nullptr");
#endif

        if (m_pNextScene)
        {
            if (m_pCurrentScene)
            {
                JPT_TERMINATE(m_pCurrentScene);
            }

            m_pCurrentScene = m_pNextScene;
            m_pNextScene = nullptr;
            m_pCurrentScene->Init();
        }

        if (m_pCurrentScene)
        {
            m_pCurrentScene->Update(deltaSeconds);
        }
    }

    void SceneManager::Terminate()
    {
        JPT_TERMINATE(m_pCurrentScene);
        JPT_TERMINATE(m_pNextScene);
    }

    void SceneManager::SwitchScene(SceneGraph* pScene)
    {
        m_pNextScene = pScene;
    }
}