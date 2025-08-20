// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"

export module jpt.SceneManager;

import jpt.TypeDefs;
import jpt.TimeTypeDefs;

namespace jpt
{
    class SceneGraph;

    /** Manages the scenes in the game as a finite state machine */
    export class SceneManager
    {
        JPT_DECLARE_SINGLETON(SceneManager);

    private:
        SceneGraph* m_pCurrentScene = nullptr;    /**< The current scene. Can be nullptr if IS_EDITOR, the Jupiter Editor will still able to run non-scene related tools */
        SceneGraph* m_pNextScene = nullptr;

    public:
        bool PreInit();
        bool Init();
        void Update(TimePrecision deltaSeconds);
        void Terminate();

        void SwitchScene(SceneGraph* pScene);
    };
}