// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Entity;

import jpt.Time.TypeDefs;

export namespace jpt
{
	class Entity
	{
	public:
		virtual ~Entity() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init() { return true; }

		virtual void Update(TimePrecision) {}

		virtual void Shutdown() {}
	};
}