// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Entity.Component.Manager;

import jpt.Entity.Component;

import jpt.HashMap;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
	/** Manages all entity components in current scene */
	class EntityComponentManager
	{
	private:
		HashMap<Id, EntityComponent*> m_components;	/**< All components in the scene */
		DynamicArray<Id> m_updatableComponents;	    /**< All components that can be updated */

	public:
		void Update(TimePrecision deltaSeconds);
	};

	void EntityComponentManager::Update(TimePrecision deltaSeconds)
	{
		for (Id id : m_updatableComponents)
		{
			EntityComponent* pComponent = m_components[id];
			UpdatableComponent* pUpdatableComponent = static_cast<UpdatableComponent*>(pComponent);

			pUpdatableComponent->Update(deltaSeconds);
		}
	}
}