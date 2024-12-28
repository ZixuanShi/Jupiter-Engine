// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene;

import jpt.Entity;
import jpt.Entity.Component.Manager;

import jpt.Time.TypeDefs;

import jpt.DynamicArray;
import jpt.String;

export namespace jpt
{
	/** Represents a game world. 
		- Entities and Components
		- Scene Scripts */
	class Scene
	{
	protected:
		String m_name;						/**< The name of the scene */
		DynamicArray<Entity*> m_entities;	/**< All entities in the scene */
		EntityComponentManager m_componentManager;	/**< Manages all entity components in current scene */

	public:
		virtual ~Scene() = default;
		
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Shutdown() {}
	};

	void Scene::Update(TimePrecision deltaSeconds)
	{
		for (Entity* pEntity : m_entities)
		{
			pEntity->Update(deltaSeconds);
		}

		m_componentManager.Update(deltaSeconds);
	}
}