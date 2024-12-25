// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt.Entity.Component;

import jpt.Time.TypeDefs;

export namespace jpt
{
	/** Base class of entity components. Not updateable */
	class EntityComponent
	{
	public:
		virtual ~EntityComponent() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }
		virtual void Shutdown() {}
	};

	/** Base class of Updateable entity components */
	class UpdatableEntityComponent : public EntityComponent
	{
	public:
		virtual void Update([[maybe_unused]] TimePrecision deltaSeconds) = 0;
	};

	template<typename TComponent>
	consteval bool IsComponentUpdatable()
	{
		return std::is_base_of_v<UpdatableEntityComponent, TComponent>;
	}
}