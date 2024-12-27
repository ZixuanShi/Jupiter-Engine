// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Scene;

import jpt.Entity;

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
		DynamicArray<Entity*> m_entities;	/**< All entities in the scene */
		String m_name;						/**< The name of the scene */

	public:
		virtual ~Scene() = default;
		
		virtual bool Init() { return true; }
		virtual void Update(TimePrecision) {}
		virtual void Shutdown() {}
	};
}