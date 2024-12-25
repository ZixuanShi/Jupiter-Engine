// Copyright Jupiter Technologies, Inc. All Rights Reserved.


export module jpt.Entity.Factory;

import jpt.Entity;

import jpt.TypeDefs;

export namespace jpt
{
	/** Factory class for creating entities
		- Assgin Id */
	class EntityFactory
	{
	private:
		inline static Id m_nextId = 0;

	public:
		template<typename TEntity>
		static TEntity* Create();
	};

	template<typename TEntity>
	TEntity* EntityFactory::Create()
	{
		TEntity* pEntity = new TEntity;
		pEntity->SetId(m_nextId);
		++m_nextId;
		return pEntity;
	}
}