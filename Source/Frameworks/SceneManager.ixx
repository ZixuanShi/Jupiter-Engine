// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

export module jpt.Scene.Manager;

import jpt.Scene.Graph;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
    /** Manages the scenes in the game as a finite state machine */
    class SceneManager
    {
        JPT_DECLARE_SINGLETON(SceneManager);

    private:
        SceneGraph* m_pCurrentScene = nullptr;    /**< The current scene. Can be nullptr if IS_EDITOR, the Jupiter Editor will still able to run non-scene related tools */
        SceneGraph* m_pNextScene = nullptr;

    public:
        bool PreInit();
        bool Init();
        void Update(TimePrecision deltaSeconds);
        void Shutdown();

        void SwitchScene(SceneGraph* pScene);
    };

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
                JPT_SHUTDOWN(m_pCurrentScene);
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

    void SceneManager::Shutdown()
    {
        JPT_SHUTDOWN(m_pCurrentScene);
        JPT_SHUTDOWN(m_pNextScene);
    }

    void SceneManager::SwitchScene(SceneGraph* pScene)
    {
        m_pNextScene = pScene;
    }
}