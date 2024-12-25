// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/CoreMacros.h"

export module jpt.Scene;

import jpt.Entity;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.HashMap;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
	/** Represents a game world
		- Container of Entities
		- Scene Script
		- UI */
	class Scene
	{
	private:
		DynamicArray<Entity*> m_entities;

	public:
		bool PreInit();
		bool Init();
		void Update(TimePrecision deltaSeconds);
		void Shutdown();
	};

	bool Scene::PreInit()
	{
		for (Entity* pEntity : m_entities)
		{
			if (!pEntity->PreInit())
			{
				return false;
			}
		}

		return true;
	}

	bool Scene::Init()
	{
		for (Entity* pEntity : m_entities)
		{
			if (!pEntity->Init())
			{
				return false;
			}
		}

		return true;
	}

	void Scene::Update(TimePrecision deltaSeconds)
	{
		for (Entity* pEntity : m_entities)
		{
			pEntity->Update(deltaSeconds);
		}
	}

	void Scene::Shutdown()
	{
		for (Entity* pEntity : m_entities)
		{
			JPT_SHUTDOWN(pEntity);
		}
	}
}