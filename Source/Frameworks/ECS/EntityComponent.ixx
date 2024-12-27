// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Entity.Component;

import jpt.Constants;
import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
	class EntityComponent
	{
	protected:
		Id m_ownerId = kInvalidValue<Id>;	/**< The entity that owns this component */

	public:
		virtual ~EntityComponent() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Shutdown() {}
	};

	class UpdatableComponent : public EntityComponent
	{
	public:
		virtual void Update(TimePrecision) = 0;
	};
}