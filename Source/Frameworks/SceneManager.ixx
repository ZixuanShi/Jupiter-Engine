// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene.Manager;

import jpt.Scene;

export namespace jpt
{
	/** Manages the scenes in the game as a state machine */
	class SceneManager
	{
	private:
		Scene* m_pCurrentScene = nullptr;
		Scene* m_pNextScene = nullptr;
	};
}