// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

export module jpt.Scene;

import jpt.Entity;

import jpt.Constants;
import jpt.HashMap;
import jpt.TypeDefs;

export namespace jpt
{
	/** Represents a game world
		- Container of Entities */
	class Scene
	{
	private:
		HashMap<Id, Entity*> m_entities;
		Id m_id = kInvalidValue<Id>;

	public:
	};
}