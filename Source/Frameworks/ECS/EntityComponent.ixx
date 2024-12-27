// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Entity.Component;

import jpt.Constants;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;
import jpt.TypeTraits;

export namespace jpt
{
	class Entity;

	/** Represents a component of an entity */
	class EntityComponent
	{
	protected:
		Id m_id = kInvalidValue<Id>; /**< Unique identifier in scene */
		Entity* pOwner = nullptr;	 /**< The entity this component belongs to */

	public:
		virtual ~EntityComponent() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Shutdown() {}
	};

	/** Represents a component that can be updated */
	class UpdatableComponent : public EntityComponent
	{
	public:
		virtual void Update(TimePrecision) = 0;
	};

	template<typename TComponent>
	concept ConceptUpdatableComponent = IsChildOf<TComponent, UpdatableComponent>;
}