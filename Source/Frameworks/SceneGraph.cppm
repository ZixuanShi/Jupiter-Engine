// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.SceneGraph;

import jpt.ObjectPool;

import jpt.Entity;
import jpt.EntityComponentManager;

import jpt.TimeTypeDefs;

import jpt.DynamicArray;
import jpt.String;

export namespace jpt
{
    /** Represents a game world. 
        - Entities and Components
        - Scene Scripts */
    class SceneGraph
    {
    protected:
        ObjectPool<Entity*> m_entityPool;       /**< Pool of pre-allocated entities */
        DynamicArray<Index> m_activeEntities;   /**< All entities id in the scene that's active. Maps to the entity pool */

        EntityComponentManager m_componentManager;    /**< Manages all entity components in current scene */
        
        String m_name;                        /**< The name of the scene */

    public:
        virtual ~SceneGraph() = default;
        
        virtual bool Init() { return true; }
        virtual void Update(TimePrecision deltaSeconds);
        virtual void Terminate() {}
    };

    void SceneGraph::Update(TimePrecision deltaSeconds)
    {
        for (Index id : m_activeEntities)
        {
            m_entityPool[id]->Update(deltaSeconds);
        }

        m_componentManager.Update(deltaSeconds);
    }
}