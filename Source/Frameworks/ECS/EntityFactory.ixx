// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Entity.Factory;

import jpt.Entity;
import jpt.TypeDefs;

export namespace jpt
{
	class EntityFactory
	{
	public:
		template<typename TEntity>
		static TEntity* Create();
	};

	template<typename TEntity>
	TEntity* EntityFactory::Create()
	{
		static Id s_nextId = 0;

		TEntity* pEntity = new TEntity();
		pEntity->SetId(s_nextId);
		++s_nextId;

		return pEntity;
	}
}