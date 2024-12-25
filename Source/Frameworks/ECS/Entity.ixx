// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

export module jpt.Entity;

import jpt.Entity.Component;

import jpt.Constants;
import jpt.DynamicArray;
import jpt.HashMap;
import jpt.TypeRegistry;
import jpt.TypeDefs;

import jpt.Time.TypeDefs;

export namespace jpt
{
	/** An object in game world. Carries components. */
	class Entity
	{
	protected:
		HashMap<Id, EntityComponent*> m_components;	/**< Components of this entity holds */
		DynamicArray<Id> m_updateableComponents;		/**< Components that should be updated */

		Id m_id = kInvalidValue<Id>;

	public:
		virtual ~Entity() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Shutdown();

	public:
		template<typename TComponent>
		void AddComponent(TComponent* pComponent = nullptr);

		template<typename TComponent>
		TComponent* GetComponent() const;

		template<typename TComponent>
		void EraseComponent();

		Id GetId() const;
	};

	void Entity::Update(TimePrecision deltaSeconds)
	{
		for (Id typeId : m_updateableComponents)
		{
			EntityComponent* pComponent = m_components[typeId];
			UpdatableEntityComponent* pUpdatableComponent = static_cast<UpdatableEntityComponent*>(pComponent);

			pUpdatableComponent->Update(deltaSeconds);
		}
	}

	void Entity::Shutdown()
	{
		for (auto& [typeId, pComponent] : m_components)
		{
			JPT_IGNORE(typeId);
			JPT_SHUTDOWN(pComponent);
		}

		m_components.Clear();
		m_updateableComponents.Clear();
	}

	template<typename TComponent>
	void Entity::AddComponent(TComponent* pComponent /* = nullptr*/)
	{
		const Id typeId = TypeRegistry::GetId<TComponent>();
		JPT_ASSERT(!m_components.Has(typeId), "Component already exists in entity");

		if (!pComponent)
		{
			pComponent = new TComponent;
			JPT_ASSERT(pComponent->Init());
		}

		m_components.Emplace(typeId, pComponent);

		if constexpr (IsComponentUpdatable<TComponent>())
		{
			m_updateableComponents.EmplaceBack(typeId);
		}
	}

	template<typename TComponent>
	TComponent* Entity::GetComponent() const
	{
		const Id typeId = TypeRegistry::GetId<TComponent>();
		JPT_ASSERT(m_components.Has(typeId), "Component does not exist in entity");
		return static_cast<TComponent*>(m_components[typeId]);
	}

	template<typename TComponent>
	void Entity::EraseComponent()
	{
		const Id typeId = TypeRegistry::GetId<TComponent>();
		JPT_ASSERT(m_components.Has(typeId), "Component does not exist in entity");

		EntityComponent* pComponent = m_components[typeId];
		JPT_SHUTDOWN(pComponent);

		m_components.Erase(typeId);

		if constexpr (IsComponentUpdatable<TComponent>())
		{
			for (size_t i = 0; i < m_updateableComponents.Count(); ++i)
			{
				if (m_updateableComponents[i] == typeId)
				{
					m_updateableComponents.Erase(i);
					break;
				}
			}
		}
	}

	size_t Entity::GetId() const
	{
		return m_id;
	}
}