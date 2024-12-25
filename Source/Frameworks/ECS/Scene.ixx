// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/CoreMacros.h"

export module jpt.Scene;

import jpt.Entity;
import jpt.Entity.Factory;

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

	public:
		template<typename TEntity>
		TEntity* AddEntity();
		Entity* GetEntity(Id id) const;
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

		m_entities.Clear();
	}

	template<typename TEntity>
	TEntity* Scene::AddEntity()
	{
		TEntity* pEntity = EntityFactory::Create<TEntity>();
		m_entities.EmplaceBack(pEntity);

		pEntity->PreInit();
		pEntity->Init();

		return pEntity;
	}

	Entity* Scene::GetEntity(Id id) const
	{
		for (Entity* pEntity : m_entities)
		{
			if (pEntity->GetId() == id)
			{
				return pEntity;
			}
		}

		return nullptr;
	}
}